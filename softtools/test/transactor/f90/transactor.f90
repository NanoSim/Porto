module Transactor
  use StorageStrategy, only : IStorageStrategy
  use StorageFactory, only : createStrategy => create
  implicit none

  private

  type, public :: TTransactor
     private
     integer :: foo
     class(IStorageStrategy), pointer :: strategy     
  end type TTransactor
  
  interface TTransactor
     module procedure construct
  end interface TTransactor

contains
  function construct () result (this)
    type(TTransactor), allocatable :: this
    allocate (this)

    call createStrategy(this%strategy)
    call this%strategy%create
  end function construct



end module Transactor
