program test
  use softf_transactor
  use iso_c_binding
  implicit none

  type (TSoftTransactor) :: t

  t = TSoftTransactor("mongodb://this.doesnt.work")
  print *, t%uri()

end program test
