// ПОСЛЕДОВАТЕЛЬНОЕ исследование с ОБЩИМ СОСТОЯНИЕМ
state = make_choice(choice1);        // Меняем общее состояние
if (!solve(state)) {                 // Все подзадачи работают с одним состоянием
    undo_choice(choice1);            // Откатываем общее состояние
}


// ИДЕАЛЬНОЕ разделение на НЕЗАВИСИМЫЕ подзадачи
result = combine(
    solve(independent_subproblem1),  // Полностью независимо
    solve(independent_subproblem2)   // Никак не влияет на subproblem1
);

