static const rb_data_type_t foo_type = {
	.wrap_struct_name = "foo",
	.function = {
		.dfree = RUBY_DEFAULT_FREE,
		.dsize = foo_size,
	},
	.flags = RUBY_TYPED_FREE_IMMEDIATELY,
};

VALUE foo_alloc(VALUE self)
{
	int* data;
	/* 割り当てて梱包します。なお型が割り当てられている必要があります */
	return Data_Make_Struct(self, int, &foo_type, data);
}
