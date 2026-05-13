# -*- coding: utf-8 -*-
import sys
import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import base64
from io import BytesIO

# --- Funkcja do generowania wykresu 2D (liniowego) ---
def generate_2d_plot(csv_path, title):
    if not os.path.exists(csv_path):
        return ""
        
    df = pd.read_csv(csv_path)
    df_int = df[df['Typ_danych'] == 'Integer']
    
    plt.figure(figsize=(9, 5))
    modes = ['Sekwencyjnie(ms)', 'Wielowatkowo(ms)', 'OpenMP(ms)', 'CUDA(ms)']
    colors = ['#3498db', '#e67e22', '#9b59b6', '#2ecc71']
    markers = ['o', 's', '^', 'D']
    
    for i, mode in enumerate(modes):
        if mode in df_int.columns:
            plt.plot(df_int['Ilosc_elementow'], df_int[mode], marker=markers[i], color=colors[i], linewidth=2, label=mode)

    plt.xlabel('Liczba elementów')
    plt.ylabel('Czas (ms)')
    plt.title(f'Wykres Liniowy 2D: {title} (dla Integer)')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    
    tmp_file = BytesIO()
    plt.savefig(tmp_file, format='png', dpi=150, bbox_inches='tight')
    plt.close()
    return base64.b64encode(tmp_file.getvalue()).decode('utf-8')

# --- Funkcja do generowania wykresu 3D ---
def generate_3d_plot(csv_path, title):
    if not os.path.exists(csv_path):
        return ""
        
    df = pd.read_csv(csv_path)
    df_int = df[df['Typ_danych'] == 'Integer']
    
    fig = plt.figure(figsize=(9, 6))
    ax = fig.add_subplot(111, projection='3d')
    
    modes = ['Sekwencyjnie(ms)', 'Wielowatkowo(ms)', 'OpenMP(ms)', 'CUDA(ms)']
    colors = ['#3498db', '#e67e22', '#9b59b6', '#2ecc71']
    xs = df_int['Ilosc_elementow'].values
    
    for i, mode in enumerate(modes):
        if mode in df_int.columns:
            ys = np.full(len(xs), i)
            zs = df_int[mode].values
            ax.plot(xs, ys, zs, color=colors[i], marker='o', linewidth=2, label=mode)

    ax.set_xlabel('Liczba elementów')
    ax.set_yticks([0, 1, 2, 3])
    ax.set_yticklabels(['Seq', 'Multi', 'OMP', 'CUDA'])
    ax.set_ylabel('Tryb')
    ax.set_zlabel('Czas (ms)')
    ax.set_title(f'Wydajność 3D: {title} (dla Integer)')
    ax.legend()
    
    tmp_file = BytesIO()
    plt.savefig(tmp_file, format='png', dpi=150, bbox_inches='tight')
    plt.close()
    return base64.b64encode(tmp_file.getvalue()).decode('utf-8')

# --- Funkcja do generowania tabeli HTML ---
def generate_html_table(csv_path):
    if not os.path.exists(csv_path):
        return "<p>Brak pliku z danymi.</p>"
        
    df = pd.read_csv(csv_path)
    
    formatters = {
        'Sekwencyjnie(ms)': lambda x: f"{x:.2f}",
        'Wielowatkowo(ms)': lambda x: f"{x:.2f}",
        'CUDA(ms)': lambda x: f"{x:.2f}",
        'OpenMP(ms)': lambda x: f"{x:.2f}"
    }
    
    return df.to_html(classes='data-table', index=False, formatters=formatters)

# --- Główny skrypt ---
if __name__ == "__main__":
    if len(sys.argv) < 5:
        print("Użycie: python SortReportGenerator.py <BubbleCSV> <MergeCSV> <QuickCSV> <OutputHTML>")
        sys.exit(1)
        
    csv_bubble = sys.argv[1]
    csv_merge = sys.argv[2]
    csv_quick = sys.argv[3]
    
    # Pobranie docelowej nazwy pliku (np. "Raport_Wydajnosci.html")
    oryginalna_nazwa = os.path.basename(sys.argv[4])
    
    # Odnalezienie ścieżki do Pulpitu użytkownika na systemie Windows
    sciezka_pulpitu = os.path.join(os.environ['USERPROFILE'], 'Desktop')
    
    # Utworzenie ostatecznej ścieżki pliku (Pulpit + Nazwa pliku)
    output_html = os.path.join(sciezka_pulpitu, oryginalna_nazwa)

    html_content = """
    <!DOCTYPE html>
    <html lang="pl">
    <head>
        <meta charset="UTF-8">
        <title>Raport Wydajności SuperComputing</title>
        <style>
            body { font-family: 'Segoe UI', Tahoma, Geneva, Arial, sans-serif; color: #333; line-height: 1.5; margin: 0 auto; padding: 20px; max-width: 1000px; background-color: #f5f7fa; }
            .report-container { background-color: #ffffff; padding: 40px; border-radius: 8px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }
            .header { background-color: #2c3e50; color: white; padding: 30px 20px; margin: -40px -40px 20px -40px; text-align: center; border-radius: 8px 8px 0 0; }
            h1 { margin: 0; font-size: 24pt; font-weight: 300; }
            h2 { border-bottom: 2px solid #3498db; padding-bottom: 5px; color: #2c3e50; margin-top: 40px; }
            h3 { color: #555; margin-top: 20px; margin-bottom: 10px; font-size: 14pt; }
            
            .chart-container { text-align: center; margin: 15px 0; }
            .chart-img { width: 100%; max-width: 800px; border: 1px solid #ecf0f1; border-radius: 5px; }
            
            table.data-table { width: 100%; border-collapse: collapse; margin: 20px 0; font-size: 10pt; }
            table.data-table th, table.data-table td { border: 1px solid #bdc3c7; padding: 10px; text-align: center; }
            table.data-table th { background-color: #34495e; color: white; font-weight: bold; }
            table.data-table tr:nth-child(even) { background-color: #f8f9fa; }
            
            @media print {
                body { background-color: white; }
                .report-container { box-shadow: none; padding: 0; }
                .header { margin: 0 0 20px 0; border-radius: 0; color: black; background-color: white; border-bottom: 2px solid black;}
                .algo-section { page-break-before: always; }
                .algo-section:first-of-type { page-break-before: auto; }
            }
        </style>
    </head>
    <body>
        <div class="report-container">
            <div class="header">
                <h1>Szczegółowy Raport Wydajności: SuperComputing</h1>
                <p>Analiza porównawcza algorytmów na architekturach CPU i GPU</p>
            </div>
    """

    algorithms = [
        ("Bubble Sort", csv_bubble),
        ("Merge Sort", csv_merge),
        ("Quick Sort", csv_quick)
    ]

    for algo_name, csv_path in algorithms:
        if os.path.exists(csv_path):
            img_2d = generate_2d_plot(csv_path, algo_name)
            img_3d = generate_3d_plot(csv_path, algo_name)
            table_html = generate_html_table(csv_path)

            html_content += f"""
            <div class="algo-section">
                <h2>{algo_name}</h2>
                <h3>Wizualizacja Złożoności (Wykres 2D)</h3>
                <div class="chart-container">
                    <img src="data:image/png;base64,{img_2d}" class="chart-img">
                </div>
                <h3>Wizualizacja Przestrzenna (Wykres 3D)</h3>
                <div class="chart-container">
                    <img src="data:image/png;base64,{img_3d}" class="chart-img">
                </div>
                <h3>Zestawienie Tabelaryczne Danych</h3>
                {table_html}
            </div>
            """

    html_content += """
        </div>
    </body>
    </html>
    """
    
    # Zapis bezpośrednio do pliku HTML z poprawnym kodowaniem znaków
    with open(output_html, 'w', encoding='utf-8') as f:
        f.write(html_content)
        
    print(f"Raport wygenerowany pomyślnie na pulpicie: {output_html}")