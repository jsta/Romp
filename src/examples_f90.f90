module examples_fortran
  use, intrinsic :: iso_c_binding
  use, intrinsic :: omp_lib, only : omp_get_num_threads, omp_get_thread_num
  implicit none
  
  contains
  
  
  subroutine f90_hello() &
  bind(C, name="f90_hello")
    integer :: tid, nthreads
    
    !$omp parallel private(tid)
    nthreads = omp_get_num_threads()
    tid = omp_get_thread_num()
    
    call intpr("Hello from thread", -1, tid, 1)
    !$omp end parallel
  end subroutine
  
  
  
  function f90_sum(x, xlen) result(s) &
  bind(C, name="f90_sum")
    real(kind=c_double) :: s
    integer(kind=c_int), intent(in), value :: xlen
    real(kind=c_double), intent(in) :: x(xlen)
    integer i
    
    s = 0.0d0
    
    !$omp parallel do default(shared) private(i) reduction(+:s)
    do i=1, xlen
      s = s + x(i)
    end do
  end function
  
  
  subroutine f90_sweep(m, n, x, vec, ret) &
  bind(C, name="f90_sweep")
    integer(kind=c_int), intent(in), value :: m, n
    real(kind=c_double), intent(inout) :: x(m, n)
    real(kind=c_double), intent(in) :: vec(m)
    real(kind=c_double), intent(out) :: ret(m, n)
    integer :: i, j
    
    !$omp parallel do default(shared) private(i, j)
    do j=1, n
      do i=1, m
        ret(i, j) = x(i, j) - vec(i)
      end do
    end do
    !$omp end parallel do
  end subroutine
  
end module
