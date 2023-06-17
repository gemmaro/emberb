#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <ruby.h>

/* 定数 */
const unsigned int win_width = 1024;
const unsigned int win_height = 768;
const unsigned int actor_size = 30;

/* 位置と方向用 */
struct vec2
{
	float x;
	float y;
};

/* プレーヤーと敵用 */
struct actor
{
	struct vec2 pos;
	struct vec2 dir;
	float speed; // ピクセル毎ミリ秒での最高速度
	SDL_Color color;
};

/* AIのアクターにAIのスクリプトのメタデータを渡すための梱包 */
struct ai_actor
{
	char* script;
	bool loaded;
	bool error;
	time_t load_time;
	struct actor* actor;
};

/* AIのエラー状態を設定し、例外を印字する可能性があります */
void ai_error(struct ai_actor* ai)
{
	ai->error = true;

	ai->actor->dir.x = 0.f;
	ai->actor->dir.y = 0.f;
	ai->actor->color.a = 127;

	/* 例外を印字 */
	VALUE exception = rb_errinfo();
	rb_set_errinfo(Qnil);

	if (RTEST(exception)) rb_warn("AI script error: %"PRIsVALUE"", rb_funcall(exception, rb_intern("full_message"), 0));
}

/* AIのエラー状態を消去 */
void ai_reset(struct ai_actor* ai)
{
	ai->error = false;
	ai->actor->color.a = 255;
}

/* （再）読み込みしたAIスクリプトを試す */
void ai_load(struct ai_actor* ai)
{
	/* スクリプトの変更日時を得る */
	struct stat script_stat;
	if (stat(ai->script, &script_stat))
	{
		if (ai->loaded)
			fprintf(stderr, "Can't stat AI script\n");
		ai->loaded = false;
		ai_error(ai);
		return;
	}

	/* 既にスクリプトを読み込んでおり更新されていなければ、することはありません */
	if (ai->loaded && ai->load_time == script_stat.st_mtime) return;

	if (ai->loaded)
		fprintf(stderr, "Reloading AI...\n");
	else
		fprintf(stderr, "Loading AI...\n");

	ai->loaded = true;
	ai->load_time = script_stat.st_mtime;

	ai_reset(ai);

	int state;
	rb_load_protect(rb_str_new_cstr(ai->script), 0, &state);

	if (state) ai_error(ai);
}

/* AIのスクリプト中で例外をrescueするためのもの */
VALUE think_wrapper(VALUE actors)
{
	rb_funcall(rb_mKernel, rb_intern("think"), 2, rb_ary_entry(actors, 0), rb_ary_entry(actors, 1));

	return Qundef;
}

/* 可能であればAIのスクリプトを走らせる */
void ai_think(struct ai_actor* ai, VALUE ai_v, VALUE player_v)
{
	if (!ai->loaded || ai->error) return;

	int state;
	rb_protect(think_wrapper, rb_ary_new_from_args(2, ai_v, player_v), &state);

	if (state) ai_error(ai);
}

/* msだけ時間が経ったらアクターを動かす */
void step_actor(struct actor* act, unsigned int ms)
{
	float norm = sqrtf(act->dir.x * act->dir.x + act->dir.y * act->dir.y);

	/* 動きなし */
	if (norm == 0.f) return;

	/* アクターがspeedに比べて遅くはあっても速くならない程度に動けるようにします */
	if (norm < 1.f) norm = 1.f;

	act->pos.x += (act->dir.x * act->speed * (float)ms) / norm;
	act->pos.y += (act->dir.y * act->speed * (float)ms) / norm;

	/* 位置が画面内に来るよう切り詰めます */
	if (act->pos.x < 0.f)
		act->pos.x = 0.f;
	else if (act->pos.x > win_width - actor_size)
		act->pos.x = win_width - actor_size;
	if (act->pos.y < 0.f)
		act->pos.y = 0.f;
	else if (act->pos.y > win_height - actor_size)
		act->pos.y = win_height - actor_size;
}

/* アクターを色付きの箱として描画します */
void draw_actor(SDL_Renderer* renderer, struct actor* act)
{
	SDL_SetRenderDrawColor(renderer, act->color.r, act->color.g, act->color.b, act->color.a);
	SDL_Rect rectangle = { .x = act->pos.x, .y = act->pos.y, .w = actor_size, .h = actor_size };
	SDL_RenderFillRect(renderer, &rectangle);
}

/* AIのスクリプト用に定義する、API用のメソッド */
/* time - 合計の消費時間をミリ秒で返します */
VALUE m_time(VALUE self)
{
	return UINT2NUM(SDL_GetTicks());
}

/* mark/free/GCなどは何ら必要ありません。クラスを定義するところにある後述のコメントを参照 */
static const rb_data_type_t actor_type = { .wrap_struct_name = "actor" };

/* Actor#pos - 画面の位置x, yをピクセルで返します */
VALUE actor_m_pos(VALUE self)
{
	struct actor* data;
	TypedData_Get_Struct(self, struct actor, &actor_type, data);

	return rb_ary_new_from_args(2, DBL2NUM(data->pos.x), DBL2NUM(data->pos.y));
}

/* Actor#dir - 直近の動く方向x, yを返します。それぞれ (-1..1) の範囲内です */
VALUE actor_m_dir(VALUE self)
{
	struct actor* data;
	TypedData_Get_Struct(self, struct actor, &actor_type, data);

	return rb_ary_new_from_args(2, DBL2NUM(data->dir.x), DBL2NUM(data->dir.y));
}

/* Actor#move - 次の動く方向を設定します。x, yはActor#posと同様です */
VALUE actor_m_move(VALUE self, VALUE x, VALUE y)
{
	float nx = NUM2DBL(x);
	float ny = NUM2DBL(y);

	struct actor* data;
	TypedData_Get_Struct(self, struct actor, &actor_type, data);

	data->dir.x = nx;
	data->dir.y = ny;

	return Qnil;
}

int main(int argc, char** argv)
{
	/* Rubyを開始 TODO これは余計だろうか */
	if (ruby_setup())
	{
		fprintf(stderr, "Failed to init Ruby VM\n");
		return 1;
	}
	/* <main>よりもいい名前を設定 */
	ruby_script("ruby");

	/* AIスクリプトで使う小振りの独自APIを定義 */
	rb_define_global_function("time", m_time, 0);

	/* ActorはRubyに渡す上で構造体actorを梱包します */
	VALUE cActor = rb_define_class("Actor", rb_cObject);
	rb_define_method(cActor, "pos", actor_m_pos, 0);
	rb_define_method(cActor, "dir", actor_m_dir, 0);
	rb_define_method(cActor, "move", actor_m_move, 2);

	/*
	   ActorがCのデータを梱包していたとしても、割り当てや解放関数を定義しなかった点に注目です。
	   これは全てのアクターをCで作り、Rubyに露出させるつもりだからです。
	   しかしRubyが新しいActorを作れてしまうと不当なデータポインタを含んでしまうので、
	   Rubyがそうできないようにすべきです。
	 */
	rb_undef_method(rb_singleton_class(cActor), "new");

	/* SDLを開始 */
	SDL_Init(SDL_INIT_VIDEO);

	/* ウィンドウを作成 */
	SDL_Window* window = SDL_CreateWindow(
		"Tag",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		win_width,
		win_height,
		0
	);
	if (window == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
		return 1;
	}

	/* 描画子を作成 */
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
		return 1;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	/* アクターを作成 */
	struct actor player = {
		.pos = { .x = win_width / 2.f + 100.f - actor_size / 2.f, .y = win_height / 2.f - actor_size / 2.f },
		.dir = { .x = 0.f, .y = 0.f },
		.speed = 0.5f,
		.color = { .r = 0, .g = 0, .b = 255, .a = 255 }
	};
	struct actor ai_act = {
		.pos = { .x = win_width / 2.f - 100.f - actor_size / 2.f, .y = win_height / 2.f - actor_size / 2.f },
		.dir = { .x = 0.f, .y = 0.f },
		.speed = 0.55f,
		.color = { .r = 255, .g = 0, .b = 0, .a = 255 }
	};

	struct ai_actor ai = {
		.script = "./ai.rb",
		.loaded = false,
		.error = false,
		.actor = &ai_act
	};

	/* アクター用のRubyオブジェクトを作成 */
	/* データがスタックにあるので解放関数にはNULLが使えます */
	VALUE player_v = TypedData_Wrap_Struct(cActor, &actor_type, &player);
	VALUE ai_v = TypedData_Wrap_Struct(cActor, &actor_type, &ai_act);

	/* プレーヤーがAIスクリプトを介して動くことがないようにします */
	rb_undef_method(rb_singleton_class(player_v), "move");

	/* タイミングを取る */
	unsigned int ai_step = 33; /* 30fpsでAIを走らせる */
	unsigned int last_time = SDL_GetTicks();
	unsigned int now;
	unsigned int frame_time;
	unsigned int ai_time;

	/* AIを起動 */
	ai_load(&ai);
	ai_think(&ai, ai_v, player_v);

	/* プレーヤーの入力用 */
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);

	/* 本体の繰り返し */
	SDL_Event event;
	bool running = true;
	while (running)
	{
		/* タイマーを更新 */
		now = SDL_GetTicks();
		frame_time = now - last_time;
		ai_time += frame_time;
		last_time = now;

		/* イベントの制御 */
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
						break;
					}
			}
		}

		/* プレーヤーの動作 */
		player.dir.x = 0.f;
		player.dir.y = 0.f;

		if (keyboard[SDL_SCANCODE_UP])
			player.dir.y -= 1.f;
		if (keyboard[SDL_SCANCODE_DOWN])
			player.dir.y += 1.f;
		if (keyboard[SDL_SCANCODE_LEFT])
			player.dir.x -= 1.f;
		if (keyboard[SDL_SCANCODE_RIGHT])
			player.dir.x += 1.f;

		/* AIの動作 */
		if (ai_time >= ai_step)
		{
			ai_load(&ai);
			ai_think(&ai, ai_v, player_v);

			ai_time %= ai_step;
		}

		/* ゲームを停止 */
		step_actor(&ai_act, frame_time);
		step_actor(&player, frame_time);

		/* 描画 */
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		draw_actor(renderer, &ai_act);
		draw_actor(renderer, &player);

		SDL_RenderPresent(renderer);

		/* CPUを休止 */
		SDL_Delay(1);
	}

	/* 整頓 */
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	/* SDLを停止 */
	SDL_Quit();

	/* Rubyを停止 */
	return ruby_cleanup(0);
}
