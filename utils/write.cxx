
#include <matrix-orbital/controller.hxx>

int
main(int argc, char *argv[])
{
    MatrixOrbitalController lcd;
    lcd.open();
    lcd.write(argv[1]);
    return 0;
}
