VALUE proc;

/* procはどこかでProcが代入された前提です */
result = rb_funcall_with_block(obj, rb_intern("each"), 0, NULL, proc);
