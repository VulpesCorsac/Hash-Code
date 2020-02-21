#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

static auto _ = [] () { ios_base::sync_with_stdio(false); cin.tie(nullptr); return 0; }();

class Book {
public:
    long long number;
    long long score;
};

class BookData {
public:
    vector < long long > books_score = {};
    vector < bool > books_scanned = {};
};

class Library {
public:
    long long number;
    long long time_to_sign_up = 0;
    long long number_of_books = 0;
    long long books_per_day = 0;

    vector < Book > books_in_library;

    long long max_value = 0;

    long long current_value = 0;
    long long max_value_left = 0;
    long long last_evaluated_book = 0;
    long long current_day_to_wait = 0;
    long long current_day_to_send = 0;
};

class LibraryAnswer {
public:
    long long day_to_sign_up = -1;
    long long books_to_send = 0;

    vector < long long > books = {};
};

class Answer {
public:
    vector < LibraryAnswer > library = {};
    vector < long long > library_number = {};

    long long score = 0;
};

BookData book_data;
vector < Library > libraries;
set < Book > scanned;
long long B, L, D;
Answer answer;

void readData() {
    cin >> B >> L >> D;
    book_data.books_score.resize(B);
    book_data.books_scanned.resize(B, false);
    libraries.resize(L);
    answer.library.resize(L);

    for (auto& book : book_data.books_score) {
        cin >> book;
    }

    long long current_number = 0;
    for (Library& library : libraries) {
        library.number = current_number++;
        cin >> library.number_of_books;
        cin >> library.time_to_sign_up;
        library.current_day_to_wait = library.time_to_sign_up;
        cin >> library.books_per_day;

        library.books_in_library.resize(library.number_of_books);
        for (Book& book : library.books_in_library) {
            cin >> book.number;
            book.score = book_data.books_score[book.number];
            library.max_value_left += book.score;
        }

        sort(library.books_in_library.begin(),
             library.books_in_library.end(),
             [](Book &lhs, Book &rhs) {
                return lhs.score > rhs.score;
             }
            );

        for (int day = 0; day < D-library.time_to_sign_up; ++day) {
            bool emptied = false;
            for (int current_book = 0; current_book < library.books_per_day; ++current_book) {
                if (library.books_per_day*day + current_book >= library.number_of_books) {
                    emptied = true;
                    break;
                }
                library.max_value += library.books_in_library[library.books_per_day*day + current_book].score;
            }
            if (emptied) {
                break;
            }
        }
    }

    /*
    for (size_t j = 0; j < libraries.size(); ++j) {
        //cerr << "Library number: " << libraries[j].number << "\n";
        for (size_t i = 0; i < libraries[j].number_of_books; ++i) {
            //cerr << libraries[j].books_in_library[i].number << " - ";
            //cerr << libraries[j].books_in_library[i].score << "\n";
        }
        //cerr << libraries[j].max_value;
        //cerr << "\n\n";
    }
    //*/

    return;
}

void generateGreedyAnswer() {
    sort(libraries.begin(),
         libraries.end(),
         [] (Library& lhs, Library& rhs) {
            return lhs.max_value > rhs.max_value;
         }
        );

    //cerr << unitbuf;
    //cerr << "First sorted\n";

    int current_library_to_sign_up = -1;
    for (int day = 0; day < D; ++day) {
        //cerr << "\nCurrent day = " << day << "\n";
        if (current_library_to_sign_up >= 0 && current_library_to_sign_up < libraries.size() && libraries[current_library_to_sign_up].current_day_to_wait > 0) {
            //cerr << "Library " << current_library_to_sign_up << " has " << libraries[current_library_to_sign_up].current_day_to_wait << " days left\n";
            --libraries[current_library_to_sign_up].current_day_to_wait;
        } else {
            if (current_library_to_sign_up >= 0 && current_library_to_sign_up < libraries.size()) {
                sort(libraries.begin()+current_library_to_sign_up+1,
                     libraries.end(),
                     [] (Library& lhs, Library& rhs) {
                        return lhs.max_value_left > rhs.max_value_left;
                     }
                    );
                //cerr << "Libraries resorted\n";
            }

            current_library_to_sign_up++;
            if (current_library_to_sign_up < L) {
                Library& current_library = libraries[current_library_to_sign_up];

                answer.library_number.push_back(current_library.number);
                answer.library[current_library.number].day_to_sign_up = day;

                //cerr << "New library " << answer.library_number[answer.library_number.size()-1] << " has been added with ";
                //cerr << current_library.current_day_to_wait << " days left at ";
                //cerr << answer.library[answer.library_number.size()-1].day_to_sign_up << " day\n";

                --current_library.current_day_to_wait;

                //cerr << "Library queue:\n";
                for (auto& number : answer.library_number) {
                    //cerr << "(" << number << " -> " << answer.library[number].day_to_sign_up << ") ";
                }
            }
        }

        for (int current_library_number = 0; current_library_number < min(current_library_to_sign_up, static_cast < int > (libraries.size())); ++current_library_number) {
            Library& current_library = libraries[current_library_number];

            if (current_library.last_evaluated_book == current_library.number_of_books) {
                //cerr << "Library " << current_library.number << " is empty\n";
                continue;
            }

            //cerr << "Searching for books to send from library " << current_library.number << "\n";
            for (int current_book = 0; current_book < current_library.books_per_day; ++current_book) {

                //cerr << "Scanned books:\n";
                for (int i = 0; i < book_data.books_scanned.size(); ++i) {
                    //cerr << "(" << i << "-" << book_data.books_scanned[i] << ") ";
                }
                //cerr << "\n";

                while (current_library.last_evaluated_book < current_library.number_of_books &&
                       book_data.books_scanned[current_library.books_in_library[current_library.last_evaluated_book].number]) {
                    ++current_library.last_evaluated_book;
                }

                //cerr << "Last evaluated book pos = " << current_library.last_evaluated_book << "\n";

                if (current_library.last_evaluated_book == current_library.number_of_books) {
                    break;
                }

                Book& book_to_send = current_library.books_in_library[current_library.last_evaluated_book];

                //cerr << "Sending book " << book_to_send.number << " ";
                book_data.books_scanned[book_to_send.number] = true;

                current_library.current_value += book_to_send.score;
                answer.score += book_to_send.score;
                answer.library[current_library.number].books.push_back(book_to_send.number);
                //cerr << " score is now: " << answer.score << "\n";

                for (Library& library : libraries) {
                    while (library.last_evaluated_book < library.number_of_books) {
                        if (!book_data.books_scanned[library.books_in_library[library.last_evaluated_book].number]) {
                            break;
                        }
                        library.max_value_left -= library.books_in_library[library.last_evaluated_book].score;
                        ++library.last_evaluated_book;
                    }
                }
            }
        }
    }
}

void printAnswer() {
    //cerr << "Score: " << answer.score << "\n";

    if (answer.library[answer.library_number.size()-1].books.size() == 0) {
        answer.library_number.resize(answer.library_number.size()-1);
    }

    cerr << answer.library_number.size() << "\n";

    for (const auto& current_library : answer.library_number) {
        if (answer.library[current_library].day_to_sign_up < 0) {
            continue;
        }

        cerr << current_library << " " << answer.library[current_library].books.size() << "\n";

        for (const auto& book : answer.library[current_library].books) {
            cerr << book << " ";
        }

        cerr << "\n";
    }
}

int main() {
    freopen("input.txt", "r+", stdin);
    freopen("output.txt", "w+", stderr);

    readData();
    generateGreedyAnswer();
    printAnswer();

    fclose(stdin);
    fclose(stdout);

    return 0;
}
