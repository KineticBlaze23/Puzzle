#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 3;
const int EMPTY = 0;
const int MAX_JUGADORES = 10;
const string ARCHIVO_JUGADORES = "jugadores.dat";

struct Player
{
    string name;
    int moves;
};

// Funciones del menú
void mostrarMenu();
void mostrarJugadores(Player jugadores[], int numJugadores);
void jugarPuzzle(Player jugadores[], int &numJugadores);
void mostrarCreditos();
void guardarJugadores(const Player jugadores[], int numJugadores);
int cargarJugadores(Player jugadores[]);

// Funciones del puzzle deslizante
void inicializarTablero(int board[SIZE][SIZE]);
void imprimirTablero(const int board[SIZE][SIZE]);
bool realizarMovimiento(int board[SIZE][SIZE], char move);
bool estaResuelto(const int board[SIZE][SIZE]);
void mezclarTablero(int board[SIZE][SIZE]);
bool esMovimientoValido(int emptyRow, int emptyCol, int newRow, int newCol);

int main()
{
    Player jugadores[MAX_JUGADORES];
    int numJugadores = cargarJugadores(jugadores);
    int eleccion;

    do
    {
        mostrarMenu();
        cin >> eleccion;

        switch (eleccion)
        {
        case 1:
            jugarPuzzle(jugadores, numJugadores);
            break;
        case 2:
            mostrarJugadores(jugadores, numJugadores);
            break;
        case 3:
            mostrarCreditos();
            break;
        case 4:
            guardarJugadores(jugadores, numJugadores);
            cout << "Saliendo...\n";
            break;
        default:
            cout << "Elección inválida. Por favor, intente de nuevo.\n";
            break;
        }
    } while (eleccion != 4);

    return 0;
}

void mostrarMenu()
{
    cout << "\nMenú:\n";
    cout << "1. Jugar Puzzle\n";
    cout << "2. Mostrar Jugadores\n";
    cout << "3. Créditos\n";
    cout << "4. Salir\n";
    cout << "Ingrese su elección: ";
}

void mostrarJugadores(Player jugadores[], int numJugadores)
{
    bool hayGanadores = false;

    for (int i = 0; i < numJugadores; ++i)
    {
        if (jugadores[i].moves > 0)
        {
            hayGanadores = true;
            break;
        }
    }

    if (!hayGanadores)
    {
        cout << "No hay jugadores ganadores para mostrar.\n";
        return;
    }

    cout << "Lista de Jugadores Ganadores:\n";
    for (int i = 0; i < numJugadores; ++i)
    {
        if (jugadores[i].moves > 0)
        {
            cout << "Nombre: " << jugadores[i].name << ", Movimientos Totales: " << jugadores[i].moves << "\n";
        }
    }
}

void jugarPuzzle(Player jugadores[], int &numJugadores)
{
    int tablero[SIZE][SIZE];
    inicializarTablero(tablero);
    mezclarTablero(tablero);

    int contadorMovimientos = 0;
    char movimiento;
    bool ganado = false;

    while (!ganado)
    {
        imprimirTablero(tablero);
        cout << "Ingrese movimiento (w/a/s/d para arriba/izquierda/abajo/derecha, q para salir): ";
        cin >> movimiento;

        if (movimiento == 'q')
        {
            cout << "Juego terminado.\n";
            return;
        }

        if (realizarMovimiento(tablero, movimiento))
        {
            contadorMovimientos++;
            ganado = estaResuelto(tablero);
        }
        else
        {
            cout << "Movimiento inválido.\n";
        }
    }

    if (ganado)
    {
        cout << "¡Felicidades! Has resuelto el puzzle en " << contadorMovimientos << " movimientos.\n";

        string nombreJugador;
        cout << "Ingrese su nombre para registrar el récord: ";
        cin >> nombreJugador;

        // Buscar si el jugador ya está en la lista
        bool encontrado = false;
        for (int i = 0; i < numJugadores; ++i)
        {
            if (jugadores[i].name == nombreJugador)
            {
                // Si el jugador ya existe, actualizar su número de movimientos
                jugadores[i].moves += contadorMovimientos;
                encontrado = true;
                break;
            }
        }

        if (!encontrado && numJugadores < MAX_JUGADORES)
        {
            // Si el jugador no existe y hay espacio, añadir un nuevo jugador
            jugadores[numJugadores].name = nombreJugador;
            jugadores[numJugadores].moves = contadorMovimientos;
            numJugadores++;
        }
        else if (!encontrado)
        {
            cout << "Lista de jugadores completa. No se puede añadir más jugadores.\n";
        }
    }
}

void mostrarCreditos()
{
    cout << "\nCréditos:\n";
    cout << "Integrantes: Fabio Pillajo\n";
    cout << "¡Gracias por jugar!\n";
}

void guardarJugadores(const Player jugadores[], int numJugadores)
{
    ofstream archivo(ARCHIVO_JUGADORES, ios::binary);
    if (!archivo)
    {
        cerr << "Error al abrir el archivo para guardar jugadores.\n";
        return;
    }

    archivo.write(reinterpret_cast<const char *>(&numJugadores), sizeof(numJugadores));
    archivo.write(reinterpret_cast<const char *>(jugadores), sizeof(Player) * numJugadores);
    archivo.close();
}

int cargarJugadores(Player jugadores[])
{
    ifstream archivo(ARCHIVO_JUGADORES, ios::binary);
    int numJugadores = 0;

    if (archivo)
    {
        archivo.read(reinterpret_cast<char *>(&numJugadores), sizeof(numJugadores));
        archivo.read(reinterpret_cast<char *>(jugadores), sizeof(Player) * numJugadores);
        archivo.close();
    }

    return numJugadores;
}

void inicializarTablero(int board[SIZE][SIZE])
{
    int num = 1;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            board[i][j] = num++;
        }
    }
    board[SIZE - 1][SIZE - 1] = EMPTY;
}

void imprimirTablero(const int board[SIZE][SIZE])
{
    cout << "+---+---+---+\n";
    for (int i = 0; i < SIZE; ++i)
    {
        cout << "|";
        for (int j = 0; j < SIZE; ++j)
        {
            if (board[i][j] == EMPTY)
            {
                cout << "   |";
            }
            else
            {
                cout << setw(2) << board[i][j] << " |";
            }
        }
        cout << "\n";
        cout << "+---+---+---+\n";
    }
}

bool realizarMovimiento(int board[SIZE][SIZE], char move)
{
    int emptyRow, emptyCol;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (board[i][j] == EMPTY)
            {
                emptyRow = i;
                emptyCol = j;
                break;
            }
        }
    }

    int newRow = emptyRow, newCol = emptyCol;

    switch (move)
    {
    case 'w':
        newRow--;
        break;
    case 's':
        newRow++;
        break;
    case 'a':
        newCol--;
        break;
    case 'd':
        newCol++;
        break;
    default:
        return false;
    }

    if (esMovimientoValido(emptyRow, emptyCol, newRow, newCol))
    {
        swap(board[emptyRow][emptyCol], board[newRow][newCol]);
        return true;
    }
    return false;
}

bool esMovimientoValido(int emptyRow, int emptyCol, int newRow, int newCol)
{
    return newRow >= 0 && newRow < SIZE && newCol >= 0 && newCol < SIZE;
}

bool estaResuelto(const int board[SIZE][SIZE])
{
    int num = 1;
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            if (i == SIZE - 1 && j == SIZE - 1)
            {
                if (board[i][j] != EMPTY)
                    return false;
            }
            else if (board[i][j] != num++)
            {
                return false;
            }
        }
    }
    return true;
}

void mezclarTablero(int board[SIZE][SIZE])
{
    srand(time(nullptr));
    for (int i = 0; i < SIZE * SIZE * 10; ++i)
    {
        char movimientos[] = {'w', 'a', 's', 'd'};
        realizarMovimiento(board, movimientos[rand() % 4]);
    }
}
