# Library GTK

Projekt **Library GTK** to aplikacja desktopowa stworzona przy użyciu biblioteki GTK, która umożliwia zarządzanie biblioteką książek. Aplikacja pozwala na dodawanie, ~~edytowanie~~, usuwanie oraz przeglądanie książek w prosty i intuicyjny sposób.

## Funkcje

- **Dodawanie książek**: Możliwość dodawania nowych książek do biblioteki.
- ~~**Edytowanie książek**: Możliwość edycji istniejących wpisów książek.~~
- **Usuwanie książek**: Możliwość usuwania książek z biblioteki.
- **Przeglądanie książek**: Przeglądanie listy wszystkich dostępnych książek.
- **Wyszukiwanie**: Funkcja wyszukiwania książek po tytule, autorze lub kategorii.

## Wymagania

Aby uruchomić projekt, potrzebujesz następujących narzędzi i bibliotek:

- **GTK 3.0**: Biblioteka do tworzenia interfejsów graficznych.
- **GCC**: Kompilator do budowania projektu.
- **Make**: Narzędzie do automatyzacji procesu budowania.

## Instalacja

1. Sklonuj repozytorium:

   ```bash
   git clone https://github.com/77Adson/library_gtk.git
   cd library_gtk
   ```

2. Zbuduj Projekt

    ```bash
    make
    ```

3. Uruchom aplikacje

    ```bash
    ./library_gtk
    ```

### Użycie

Po uruchomieniu aplikacji, zostaniesz powitany przez główne okno aplikacji, gdzie możesz:

- **Wczytać zawartość savefile.txt:** Kliknij przycisk "Wczytaj"

- **Zapisać dane książek do savefile.txt:** Kliknij "Zapisz"

- **Dodać nową książkę:** Kliknij przycisk "Dodaj książkę" i wypełnij formularz.

- ~~**Edytować książkę:** Wybierz książkę z listy i kliknij "Edytuj".~~

- **Usunąć książkę:** Wybierz książkę z listy i kliknij "Usuń".

- **Przeglądać książki:** Lista książek jest wyświetlana na głównym ekranie.

- **Wyszukać książkę:** Użyj pola wyszukiwania, aby znaleźć konkretną książkę.

## Struktura projektu

- **`src/`**: Zawiera kod źródłowy aplikacji.
- ~~**`include/`**: Zawiera pliki nagłówkowe.~~
- **`Makefile`**: Plik do automatyzacji procesu budowania.

## Autor

Projekt został stworzony przez [77Adson](https://github.com/77Adson).