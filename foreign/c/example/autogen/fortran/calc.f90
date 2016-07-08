module calc
  use iso_c_binding
  use simple
  implicit none

contains
  subroutine calc_fn(sref) bind(c)
    use iso_c_binding
    use simple
    use softf_allocatable_module
    implicit none
    type(c_ptr), value :: sref
    type(TSimple) :: my_simple                 !! DENNE

    my_simple = TSimple(sref)
    print *, 'mysimple',my_simple%dimensions%NI, my_simple%dimensions%NJ
    print *, my_simple%vec(1), my_simple%vec(2)
    print *, my_simple%matrix(1,1), my_simple%matrix(1,2), my_simple%matrix(2,1)
    my_simple%test(1) = 42
  end subroutine calc_fn
end module calc
