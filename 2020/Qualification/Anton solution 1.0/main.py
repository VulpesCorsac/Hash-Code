from typing import List, Set


class Library:
    def __init__(self, id: int, days_to_sign_up: int, books: List[int], books_per_day: int):
        self.id = int(id)
        self.days_to_sign_up = days_to_sign_up
        self.books = books
        self.books_per_day = books_per_day

    def scanned_books(self, days_to_deadline: int, books_awards: dict) -> List[int]:

        # Первая стратегия, сортируем все книги в порядке убывания стоимости, в начале сканируем самые дорогие потом
        # самые дешовые

        return sorted(self.books, key=lambda x: books_awards[x], reverse=True)


def get_libraries(list_of_libraries_params: List[int]) -> List[Library]:
    libraries = []
    for idx in range(0, len(list_of_libraries_params), 2):
        libraries.append(Library(id=idx/2,
                                 days_to_sign_up=list_of_libraries_params[idx][1],
                                 books=list_of_libraries_params[idx+1],
                                 books_per_day=list_of_libraries_params[idx][1]))
    return libraries


if __name__ == '__main__':
    example_book = r'submission_b.txt'
    lines = []
    with open(example_book) as fp:
        Lines = fp.readlines()
        for line in Lines:
            lines.append([int(x) for x in line.split()])
    del lines[-1]


    # Определение основных параметров
    days_to_deadline = lines[0][-1]
    books_awards = {k: v for k, v in enumerate(lines[1])}
    libraries = get_libraries(lines[2:])

    # отсортируем библиотеки по скорости логина
    sorted_libraries = sorted(libraries, key=lambda x: x.days_to_sign_up, reverse=True)

    # Получаем список ID библиотек, в порядке котором будем с ними работать
    libraries_to_word = []
    days_to_sign_up = 0
    for lbr in sorted_libraries:
        if days_to_sign_up < days_to_deadline:
            libraries_to_word.append(lbr)
            days_to_sign_up += lbr.days_to_sign_up

    output_file = [[len(libraries_to_word)], ]
    # работаем только с библиотеками которые у нас попали в список
    days_to_finish = days_to_deadline


    for lbr in libraries_to_word:
        books_to_scan = lbr.scanned_books(days_to_deadline=days_to_finish-lbr.days_to_sign_up,
                                          books_awards=books_awards)


        output_file.append([lbr.id, len(books_to_scan)])
        output_file.append(books_to_scan)



    with open("submission_b_last.txt", 'w') as f:
        for _ in output_file:
            f.write((" ").join([str(x) for x in _]))
            f.write('\n')
