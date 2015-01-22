module MongoStrategy
  use StorageStrategy, only : IStorageStrategy
  implicit none
  private

  type, extends(IStorageStrategy), public :: TMongoStrategy
   contains
     procedure :: create
  end type TMongoStrategy

contains
  subroutine create (this)
    class (TMongoStrategy) :: this
    print *, 'hello from mongo'

  end subroutine create

end module MongoStrategy
