module StorageFactory
  use StorageStrategy, only : IStorageStrategy
  use MongoStrategy, only : TMongoStrategy
  implicit none
  private

  interface create
     module procedure StorageFactory_create
  end interface create

  public :: create
contains
  subroutine StorageFactory_create (strategy)
    class (IStorageStrategy), pointer, intent(inout)   :: strategy
    class (TMongoStrategy), allocatable, target :: str
    allocate (str)
    strategy => str
    call strategy%create()

  end subroutine StorageFactory_create
end module StorageFactory
