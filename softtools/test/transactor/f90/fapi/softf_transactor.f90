module softf_transactor
  use iso_c_binding
  implicit none
  private

  interface softc_strlen
     function softc_strlen(cstring) bind (c)
       use iso_c_binding, only : C_PTR, C_SIZE_T
       implicit none
       type (C_PTR), value                :: cstring
       integer (kind=c_size_t)            :: softc_strlen
     end function softc_strlen
  end interface softc_strlen
  
  type, public :: TSoftTransactor
     private
     type (C_PTR) :: ref    
   contains
     procedure, public :: uri
  end type TSoftTransactor

  interface TSoftTransactor
     module procedure constructor
  end interface TSoftTransactor
  
contains
  function constructor(uri)
    use iso_c_binding
    implicit none
    !
    interface 
       function softc_transactor_new(uri) bind (c)
         use iso_c_binding, only : c_char, c_ptr
         implicit none
         character(kind=c_char), intent(in)    :: uri(*)
         type (c_ptr)                          :: softc_transactor_new
       end function softc_transactor_new
    end interface
    !
    character(len=*,kind=c_char), intent(in)    :: uri
    type(TSoftTransactor), allocatable          :: constructor

    allocate(constructor)
    constructor%ref = softc_transactor_new(TRIM(uri) // C_NULL_CHAR)
  end function constructor

  function uri (this)
    use iso_c_binding    
    implicit none
    
    interface 
       function softc_transactor_get_uri(ref) bind (c)
         use iso_c_binding, only : C_PTR
         implicit none
         type (C_PTR), value, intent(in)      :: ref
         type (C_PTR)                         :: softc_transactor_get_uri
       end function softc_transactor_get_uri
    end interface

    class(TSoftTransactor)           :: this
    character, pointer, dimension(:) :: uri
    type (C_PTR)                     :: cstr
    integer(kind=c_size_t)           :: stringlen(1)

    cstr     = softc_transactor_get_uri (this%ref)
    stringlen = softc_strlen(cstr)
    call C_F_POINTER(cstr, uri, stringlen)   

  end function uri

end module softf_transactor
