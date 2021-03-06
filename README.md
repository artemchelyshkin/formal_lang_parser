# formal_lang_parser

Домашнее задание №5, вариант 19

Задача:
Даны alpha и буква x. Найти максимальное k, такое что в L есть слова, заканчивающиеся на x^k. alpha - регулярное
выражение в обратной польской записи, alphabet = {a, b, c}.

Решение:
Обрабатываем выражение - заводим стек; каждый элемент, хранящийся в стеке - язык, распознаваемый каким-то регулярным 
выражением. Для данной задачи про каждый язык нам достаточно знать лишь 2 вещи: max k такое, что есть слова 
закнчивающиеся на x^k и max l такая, что есть слова имееющие вид x^l (если в языке нет слов такого вида, то l = -1). 
Изначалльно стек пуст. 
Проходим по всей строке с регулярным выражением: 
1. если встретили 1 или элемент алфавита, создадим новый экземпляр класса языков, для него тривиально можно подсчитать 
оба необходимых нам значения.
2. если это оператор * , то вытаскиваем из стека один элемент. Если в данном языке была ненулевая строка из x, то взяв 
звезду Клини от этого языка, мы можем получить строку из x бесконечной длины, а также и бесконечный суффикс. Если там не 
было ни одной строки из x, то мы можем сделать такую, взяв этот язык 0 раз и получив пустое слово. Наибольший суффикс 
подсчитывается легко: он поменяется лишь в том случае, если в языке была строка из x.
3. если это оператор +, то вытаскиваем из стека 2 элемента. В новом языке наибольшая строка подсчитвается нахождением
максимамума из 2, аналогично для наибольшего суффикса.
4. если это оператор ., то вытаскиваем из стека 2 элемента. В новом языке наибольшая строка = наибольшая строка 1 языка +
наибольшая строка 2 языка (если те языки содержат строки из x, если нет, то -1). Наибольший суффикс = max( наибольший 
суффикс 2 языка, наибольшая строка из x 2 языка + наибольший суффикс из 1 языка). Если во 2 языке не было строк из x, то
максимальный суффикс равен максимальному суффиксу 2 языка.
Оставшийся язык на стеке после прочтения всех символов - наш язык. Смортим его наибольший суффикс - это и есть ответ.

Доказательство:
Индукция по числу операций:
База, n = 0:
Символы + '1' обрабатываются тривиально, для них наибольший суффикс и наибольшая строка обрабатывается верно, очевидно.
Шаг, n = k + 1:
Рассмотрим последнюю операцию в порядке действий. Эта опреация будет проводиться над языками, для которых верно 
посчитаны эти 2 значения. Подсчет значений в новом языке для разных операций описан в решении, корректонсть их работы 
очевидна.

Сложность:
O(n), n - длина входной строки. Рассматриваем каждый символ строки, на каждом шаге делаем максимум 3 опреации со стеком 
за O(1). 
