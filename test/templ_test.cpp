#include <templ.h>

int main() {
  templ_class obj;

  if (obj.add(1, 2) == 3) {
    return 0;
  } else {
    return 1;
  }
}
