// Лабораторная работа: решение СЛАУ методом Зейделя
// Точность eps = 0.01
#include <iostream>
#include <iomanip>
#include <clocale>
#include <cstdlib>   // для system("pause")
using namespace std;

const int N = 4;             // размер системы
const double EPS = 0.01;     // заданная точность
const int MAX_ITER = 1000;   // защита от бесконечного цикла

// Модуль числа (пишем сами, без библиотек)
double modul(double x)
{
    if (x < 0)
        return -x;
    return x;
}

// Проверка 1: на главной диагонали не должно быть нулей (на них делим)
bool netNuleyNaDiagonali(double a[N][N])
{
    for (int i = 0; i < N; i++)
        if (a[i][i] == 0)
            return false;
    return true;
}

// Проверка 2: диагональное преобладание (условие сходимости метода):
// |a[i][i]| > сумма модулей остальных элементов строки
bool diagonalnoePreobladanie(double a[N][N])
{
    for (int i = 0; i < N; i++)
    {
        double summa = 0;
        for (int j = 0; j < N; j++)
            if (j != i)
                summa = summa + modul(a[i][j]);
        if (modul(a[i][i]) <= summa)
            return false;
    }
    return true;
}

// Вывод системы на экран
void vyvodSistemy(double a[N][N], double b[N])
{
    cout << "Исходная система:" << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (a[i][j] < 0) cout << " - ";
            else             cout << " + ";
            cout << fixed << setprecision(4) << modul(a[i][j]) << "*x" << j + 1;
        }
        cout << " = " << b[i] << endl;
    }
    cout << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    double a[N][N] = {
        {25.1000, 0.1063,  0.1276,  0.1488},
        { 0.0801, 24.2000, 0.1225,  0.1437},
        { 0.0750, 0.0963,  23.3000, 0.1387},
        { 0.0700, 0.0912,  0.1124,  22.4000}
    };
    double b[N] = {56.2234, 58.9772, 61.3645, 63.3853};

    double xStar[N];   // приближение с прошлого шага  x(k)
    double xNov[N];    // новое приближение            x(k+1)

    vyvodSistemy(a, b);

    if (!netNuleyNaDiagonali(a))
    {
        cout << "Ошибка: на диагонали есть ноль, метод применить нельзя." << endl;
        return 0;
    }
    if (diagonalnoePreobladanie(a))
        cout << "Диагональное преобладание есть -> метод сходится." << endl << endl;
    else
        cout << "Внимание: нет диагонального преобладания, сходимость не гарантирована." << endl << endl;

    // Начальное приближение: x(0)[i] = b[i] / a[i][i]
    for (int i = 0; i < N; i++)
        xStar[i] = b[i] / a[i][i];

    cout << "Решение по методу Зейделя:" << endl;
    cout << " k        x1        x2        x3        x4     погрешность" << endl;
    cout << " 0";
    for (int i = 0; i < N; i++)
        cout << setw(10) << setprecision(4) << xStar[i];
    cout << endl;

    int k = 0;           // номер итерации
    double pogr;         // max |x(k+1) - x(k)|

    do
    {
        for (int i = 0; i < N; i++)
        {
            double summa = 0;
            // ГЛАВНОЕ ОТЛИЧИЕ ОТ ЯКОБИ:
            // для j < i берём уже посчитанные НОВЫЕ значения xNov
            for (int j = 0; j < i; j++)
                summa = summa + a[i][j] * xNov[j];
            // для j > i новых значений ещё нет, берём старые xStar
            for (int j = i + 1; j < N; j++)
                summa = summa + a[i][j] * xStar[j];
            xNov[i] = (b[i] - summa) / a[i][i];
        }

        // Погрешность — наибольшая разница между шагами
        pogr = 0;
        for (int i = 0; i < N; i++)
        {
            double raznica = modul(xNov[i] - xStar[i]);
            if (raznica > pogr)
                pogr = raznica;
        }

        // Новое приближение становится старым
        for (int i = 0; i < N; i++)
            xStar[i] = xNov[i];

        k++;

        cout << setw(2) << k;
        for (int i = 0; i < N; i++)
            cout << setw(10) << setprecision(4) << xNov[i];
        cout << setw(13) << setprecision(6) << pogr << endl;

    } while (pogr > EPS && k < MAX_ITER);

    cout << endl << "Ответ (eps = " << EPS << "):" << endl;
    for (int i = 0; i < N; i++)
        cout << "x" << i + 1 << " = " << setprecision(3) << xNov[i] << endl;
    cout << "Число итераций = " << k << endl << endl;

    // Проверка: подставляем ответ в систему, считаем невязку r = A*x - b
    cout << "Проверка (невязка r = A*x - b):" << endl;
    for (int i = 0; i < N; i++)
    {
        double levaya = 0;
        for (int j = 0; j < N; j++)
            levaya = levaya + a[i][j] * xNov[j];
        cout << "r" << i + 1 << " = " << setprecision(6) << levaya - b[i] << endl;
    }

    system("pause");
    return 0;
}
