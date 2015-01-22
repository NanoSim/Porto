#include <softc-transactor.h>
#include <iostream>

int main()
{
  softc_transactor_t *t = softc_transactor_new("mongodb://localhost");
  std::cout << softc_transactor_get_uri(t) << std::endl;
  return 0;
}
