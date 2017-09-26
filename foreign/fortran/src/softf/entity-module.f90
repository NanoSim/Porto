module entity_mod
  use, intrinsic :: iso_c_binding, only : C_PTR
  private

  interface softc_strlen
     function softc_strlen(cstring) bind (c)
       use, intrinsic :: iso_c_binding, only : C_PTR, C_SIZE_T
       implicit none
       type (C_PTR), value                :: cstring
       integer (kind=c_size_t)            :: softc_strlen
     end function softc_strlen
  end interface softc_strlen

  public :: TEntity
  type, abstract :: TEntity     
     type (C_PTR) :: ref
   contains     
     procedure, public, pass(self) :: get_id => entity_get_id

  end type TEntity

contains
  function entity_get_id(self)
    use, intrinsic :: iso_c_binding
    implicit none

    interface
       function softc_entity_get_id(ref) bind (c)
         use, intrinsic ::  iso_c_binding, only : C_PTR
         implicit none
         type (C_PTR), value, intent(in) :: ref
         type (C_PTR)                    :: softc_entity_get_id
       end function softc_entity_get_id
    end interface

    class (TEntity) :: self
    character, pointer, dimension (:) :: entity_get_id
    type (C_PTR)                      :: cstr
    integer (kind=C_SIZE_T)           :: stringlen(1)

    cstr = softc_entity_get_id (self%ref)
    stringlen = softc_strlen (cstr)
    call C_F_POINTER (cstr, entity_get_id, stringlen)
  end function entity_get_id
end module entity_mod
