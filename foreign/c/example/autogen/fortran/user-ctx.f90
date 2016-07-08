module user_ctx_module
  use iso_c_binding
  implicit none

  type, bind(c) :: user_ctx
     integer(c_int32_t) :: a
     type(c_ptr)        :: simple
  end type user_ctx

end module user_ctx_module
