PROGRAM hello
  use Transactor, only : TTransactor
  implicit none

  type (TTransactor) :: tr

  tr = TTransactor()

!  call tr % greet()
END PROGRAM hello
