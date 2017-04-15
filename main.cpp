#include <bits/stdc++.h>

using namespace std;



const int N = 1000;
int n, m, L[N], worth_line, iter = 0;
double  x[1000], B[1000], sx_tab[N][N], delta[N], v[N], base[N], z[N], Lb[N], solve = 0, value = 0;
int  l[1000], ans[1000];
double target[N], best;


/*Переписать форматер в паттерн SingleTone*/
struct Formater {
    std::vector<std::pair<std::string, bool> > strings;
    std::size_t width;

    Formater() {
        cout.setf(ios::fixed | ios::showpoint);
        cout.precision(2);
    }

    ~Formater() {
        flush();
    }

    template <typename T>
    void print(const T &t, bool resize = true) {
        std::stringstream ss;
        ss.setf(ios::fixed | ios::showpoint);
        ss.precision(2);
        ss << t;
        strings.emplace_back(ss.str(), resize);
    }

    void flush() {
        width = 0;
        for (auto str : strings) {
            if (str.second) {
                width = std::max(width, str.first.length());
            }
        }
        width += 1;

        for (auto &str : strings) {
            if (str.second)
                std::cout << std::string(width - str.first.length(), ' ') + str.first << " ";
            else
            std::cout << str.first << " ";
        }

        strings.clear();
    }
} formater;

/*Вычисление оценки*/
void compute_value(int j) {
    value = 1;
    for (int i = 0; i < n + m; i++) {
        value += (-1) * v[i] * x[i] - z[i];
    }
//    value -= z[j];
    formater.print("Value: \n", false);
    formater.print(value);
    formater.print("\n\n");
}

/*Создаём таблицу*/
void create_table() {

    for (int i = 0; i < n; i++) {
        base[i] = -200;
    }

    for (int i = 0; i < n; i++) {
        sx_tab[i][0] = B[i];
    }

    for (int i = n; i < n + m; i++) {
        sx_tab[i][0] = Lb[i - n];
    }

    for (int i = 0; i < n; i++) {
        delta[i] = -100;
    }
    for (int i = n; i < n + m; i++) {
        delta[i] = 0;
    }
    for (int i = 0; i < n; i++) {
        sx_tab[n + m][i + 1] = delta[i];
    }

    for (int i = 0; i < n + m; i++) {
        sx_tab[i][i + 1] = 1;
    }


}

/*Выводим таблицу на экран*/
void print_table() {

    /*Глобально делать и в евалуате ++*/

    formater.print("Table: \n", false);
    formater.print("Iteration = ", false);
    formater.print(iter, false);
    formater.print("\n");
    int k = 0;
    for (int i = 0; i < n + m + 1; i++) {
        formater.print(k + 1);
        for (int j = 0; j < n + m + 2; j++) {
            formater.print(sx_tab[i][j]);
        }
        formater.print("\n");
        k++;
    }

    formater.print("\n");
    formater.print("Vect: \n", false);
    for (int i = 0; i < n; i++) {
        formater.print((-1) * v[i]);
    }

    formater.print("\n\n");
    formater.print("Solve = \n", false);
    formater.print(solve);
    formater.print("\n\n");
}

/*Вычисляем z*/
void evaluate_z() {

    for (int i = 0; i < n + m; i++) {
        z[i] = 0;
        for (int j = 0; j < n + m + 1; j++) {
            z[i] += sx_tab[i][j + 1] * x[j];
        }
    }

    /*Заменить на z[i]*/
    for (int i = 0; i < n + m; i++) {
        sx_tab[i][n + m + 1] = z[i];
    }
}

/*Линейный раскрой*/
void linear_cut(int length) {
    for (int i = 0; i <= length; i++) {
        ans[i] = 0;
        target[i] = 0;
        for (int j = 0; j < n; j++) {
            if (l[j] <= i && (target[i] < target[i - l[j]] + (-1) * v[j])) {
                target[i] = target[i - l[j]] + (-1) * v[j];
                ans[i] = j + 1;
            }
        }
    }
}

/*Расшифровка ответа линейного раскроя*/
void transformation_linear_cut(int length) {

    for (int i = 0; i < n; i++) {
        x[i] = 0;
    }
    int y = length;
    while (ans[y] > 0) {
        x[ans[y] - 1]++;
        y -= l[ans[y] - 1];
    }
    formater.print("Вошли\n", false);
    for (int i = 0; i < m; i++) {
        formater.print("Линейный раскрой для каждой длины: ", false);
        formater.print(x[i]);
        formater.print("\n\n");
    }
    formater.print("\n");
    formater.print("Vect: \n", false);
    for (int i = 0; i < n; i++) {
        formater.print((-1) * v[i]);
    }
    formater.print("Вышли\n", false);

}

/*Переименовать*/
double evaluate_solve() {

    solve = 0;
    for (int i = 0; i < n + m; i++) {
        solve += z[i] * (-1) * delta[i];
    }

    return solve;
}

void check_base() {

    for (int i = 0; i < n + m; i++) {
        if (base[i] != -200 && delta[i] != 0) {
            delta[i] = -1;
        }
    }
    /*Таже самая лажа, что и с z*/
    for (int i = 0; i < n; i++) {
        sx_tab[n + m + 1][i + 1] = delta[i];
    }
}

/*Вычисляем двойственные переменные*/
void evaluate_dual_variables() {

  for (int i = 0; i < n + m; i++) {
        v[i] = 0;
        for (int j = 0; j < n + m; j++) {
            v[i] += delta[j] * sx_tab[j][i + 1];
        }
    }

    formater.print("Dual: ", false);
    for (int i = 0; i < n + m; i++) {
        formater.print(v[i]);
    }
    formater.print("\n\n");
}

/*Находим индекс подходящего элемента в стоблце*/
void find_best() {
    int min = INT_MAX;
    best = (double)(min_element(z, z + n + m) - z);
    for (int i = 0; i < n; i++) {
        if (z[i] < min && z[i] != 0) {
            best = i;
            /*min = z[i];*/
        }
    }
}

/*Находим строку, которая будет ведущей*/
int find_worth_line() {

    worth_line = -1;
    double worth_value = INT_MAX;

    for (int i = 0; i < n + m; i++) {
        if (sx_tab[i][n + m + 1] > 0 && worth_value > sx_tab[i][0] / sx_tab[i][n + m + 1]) {
            worth_value = sx_tab[i][0] / sx_tab[i][n + m + 1];
            worth_line = i;
        }
    }

    base[worth_line] = 5;
    return worth_line;
}

/*Вычисляем таблицу*/
bool evaluate_table() {

    check_base();
    evaluate_dual_variables();
    for (int i = 0; i < m; i++) {
        linear_cut(L[i]);
        compute_value(i);
        transformation_linear_cut(L[i]);
    }
    evaluate_z();
    find_best();
    int worth = find_worth_line();
    if (worth == -1) {
        cout << "Функция неограничена сверху, решения не существует" << endl;
        return false;
    }
    if (evaluate_solve() - 1 <= 1e-6) {
        return false;
    }
    iter++;
    double coefficient = sx_tab[worth_line][n + m + 1];
    for (int i = 0; i < n + m + 1; i++) {
        sx_tab[worth_line][i] /= coefficient;
    }

    for (int i = 0; i < n + m; i++) {
        if (i == worth_line) continue;
        coefficient = sx_tab[i][n + m + 1];
        for (int j = 0; j < n + m + 1; j++) {
            sx_tab[i][j] -= coefficient * sx_tab[worth_line][j];
        }
    }

    return true;
}

int main() {

    freopen("input", "r", stdin);

    freopen("output", "w", stdout);

    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> l[i];
        cin >> B[i];
    }

    for (int i = 0; i < m; i++) {
        cin >> L[i];
        cin >> Lb[i];
    }
//    for (int i = 0; i < n; i++) {
//        cin >> B[i];
//    }
//    cout.precision(2);
//    cout.width(10);
    unsigned int start_time = clock();
    create_table();
    print_table();
    while(evaluate_table());
    double value = 0;
    for (int i = 0; i < n; i++) {
        value += sx_tab[i][0];
    }
    formater.print("Finish table", false);
    formater.print("\n\n");
    print_table();
    formater.print("Volume of cut: \n", false);
    formater.print(value);
    formater.print("\n");
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    formater.print("Time of execution: ", false);
    formater.print((double)search_time/CLOCKS_PER_SEC);
    formater.print("\n");
    return 0;
}
