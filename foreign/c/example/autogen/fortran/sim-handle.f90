module sim_handle_module
  use iso_c_binding
  implicit none
  
  type, bind(c) :: sim_handle
     type(c_ptr) :: user_context;
  end type sim_handle
end module sim_handle_module
