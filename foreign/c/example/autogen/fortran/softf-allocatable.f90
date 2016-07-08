module softf_allocatable_module
  use, intrinsic :: iso_c_binding
  implicit none

  type, bind(c) :: softf_block
     integer(c_size_t) :: rank
     type(c_ptr)       :: dims
     type(c_ptr)       :: data
  end type softf_block

  type, bind(c) :: softf_allocatable
     type(c_ptr)       :: block  ! softf_block
     type(c_ptr)       :: data   !     
  end type softf_allocatable

end module softf_allocatable_module
