module StorageStrategy
  implicit none
  private

  type, abstract, public :: IStorageStrategy
     private
   contains
     procedure :: create
  end type IStorageStrategy

  abstract interface
     subroutine create(this)
       import IStorageStrategy
       class(IStorageStrategy) :: this          
     end subroutine create
  end interface
end module StorageStrategy
