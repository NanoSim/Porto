module demo
  use iso_c_binding
  use sim_handle_module
  use user_ctx_module
  use simple
  implicit none

contains
  subroutine demo_compute(handle, user_data) bind(c)
    use iso_c_binding
    implicit none
    type(c_ptr), value        :: handle
    type(c_ptr), value        :: user_data
    type(sim_handle), pointer :: hdl
    type(user_ctx), pointer   :: ctx
    type(TSimple)             :: simple
    !
    integer :: i, j
    real(c_double) :: tmp
    call c_f_pointer(handle, hdl)
    call c_f_pointer(hdl%user_context, ctx)
    print *, 'hello from demo-compute', ctx%a

    ! Setup entity 
    simple = TSimple(ctx%simple)
    
    ! Multiply matrix A and vector x
    do j = 1, simple%dimensions%NJ
       tmp = 0.0
       do i = 1, simple%dimensions%NI
          tmp = tmp + simple%A(i,j)*simple%x(i)
       end do
       simple%Ax(j) = tmp
    end do
  end subroutine demo_compute
  
end module demo
