import pandas as pd
import matplotlib.pyplot as plt
import os
import sys


def main():

    FILE_NAME = 'wyniki_sortowania_bubble.csv'
    OUTPUT_FILE = 'wykres_wydajnosci.png'
    TYPE = ''

    if len(sys.argv) > 1:
        FILE_NAME = sys.argv[1]
        OUTPUT_FILE = sys.argv[2] if len(sys.argv) > 2 else 'wykres_wydajnosci.png'
        TYPE = sys.argv[3] if len(sys.argv) > 3 else ''

    if not os.path.exists(FILE_NAME):
        print(f"Błąd: Nie znaleziono pliku '{FILE_NAME}'.")
        return

    # 1. Wczytanie danych z CSV
    df = pd.read_csv(FILE_NAME)

    # --- KULOODPORNE CZYSZCZENIE DANYCH ---
    df['Sekwencyjnie(ms)'] = pd.to_numeric(df['Sekwencyjnie(ms)'], errors='coerce')
    df['Wielowatkowo(ms)'] = pd.to_numeric(df['Wielowatkowo(ms)'], errors='coerce')
    df['CUDA(ms)'] = pd.to_numeric(df['CUDA(ms)'], errors='coerce')
    df['OpenMP(ms)'] = pd.to_numeric(df['OpenMP(ms)'], errors='coerce') # NOWA KOLUMNA

    # Wyrzucamy wszystkie błędne/puste wiersze
    df = df.dropna()

    # Uśrednienie wyników
    df_avg = df.groupby(['Ilosc_elementow', 'Typ_danych']).mean(numeric_only=True).reset_index()

    # Rozdzielenie danych na Integer i Float
    df_int = df_avg[df_avg['Typ_danych'] == 'Integer'].sort_values('Ilosc_elementow')
    df_float = df_avg[df_avg['Typ_danych'] == 'Float'].sort_values('Ilosc_elementow')

    # 2. Przygotowanie okna z dwoma wykresami
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 6))
    TITLE: str = f"Porównanie wydajności algorytmów {TYPE}"
    fig.suptitle(TITLE, fontsize=16)

    # --- Wykres 1: INTEGER ---
    if not df_int.empty:
        ax1.plot(df_int['Ilosc_elementow'], df_int['Sekwencyjnie(ms)'], marker='o', linewidth=2, label='Sekwencyjne')
        ax1.plot(df_int['Ilosc_elementow'], df_int['Wielowatkowo(ms)'], marker='s', linewidth=2, label='Wielowątkowe (C++20)')
        ax1.plot(df_int['Ilosc_elementow'], df_int['OpenMP(ms)'], marker='d', linewidth=2, label='OpenMP') # NOWA LINIA
        ax1.plot(df_int['Ilosc_elementow'], df_int['CUDA(ms)'], marker='^', linewidth=2, label='CUDA (GPU)')
        
        ax1.set_title('Sortowanie - Typ Integer', fontsize=14)
        ax1.set_xlabel('Ilość elementów', fontsize=12)
        ax1.set_ylabel('Czas [ms] (Skala logarytmiczna)', fontsize=12)
        ax1.set_yscale('log')
        ax1.grid(True, which="both", ls="--", alpha=0.5)
        ax1.legend(fontsize=11)

    # --- Wykres 2: FLOAT ---
    if not df_float.empty:
        ax2.plot(df_float['Ilosc_elementow'], df_float['Sekwencyjnie(ms)'], marker='o', linewidth=2, label='Sekwencyjne')
        ax2.plot(df_float['Ilosc_elementow'], df_float['Wielowatkowo(ms)'], marker='s', linewidth=2, label='Wielowątkowe (C++20)')
        ax2.plot(df_float['Ilosc_elementow'], df_float['OpenMP(ms)'], marker='d', linewidth=2, label='OpenMP') # NOWA LINIA
        ax2.plot(df_float['Ilosc_elementow'], df_float['CUDA(ms)'], marker='^', linewidth=2, label='CUDA (GPU)')
        
        ax2.set_title('Sortowanie - Typ Float', fontsize=14)
        ax2.set_xlabel('Ilość elementów', fontsize=12)
        ax2.set_ylabel('Czas [ms] (Skala logarytmiczna)', fontsize=12)
        ax2.set_yscale('log')
        ax2.grid(True, which="both", ls="--", alpha=0.5)
        ax2.legend(fontsize=11)

    # 3. Rysowanie i zapis
    plt.tight_layout()
    plt.savefig(OUTPUT_FILE, dpi=300)
    print("Wykres zostal zapisany jako 'wykres_wydajnosci.png'")

if __name__ == '__main__':
    main()