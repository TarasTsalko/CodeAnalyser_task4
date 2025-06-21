class DataProcessor:
    """
    Класс для обработки данных различной сложности
    """
    
    def __init__(self, initial_data=None):
        """
        Инициализация объекта
        """
        self.data = initial_data if initial_data is not None else []
        self.processed = False

    # Короткий метод
    def add_data(self, value):
        """Добавить значение в данные"""
        self.data.append(value)

    # Средний по длине метод
    def clean_data(self):
        """
        Очистка данных от пустых значений
        """
        self.data = [item for item in self.data if item is not None]
        self.processed = True

    # Длинный метод с комплексной логикой
    def process_data(self):
        """
        Обработка данных с применением различных преобразований
        """
        if not self.processed:
            self.clean_data()
            
        result = []
        for item in self.data:
            if isinstance(item, int):
                result.append(item * 2)
            elif isinstance(item, str):
                result.append(item.upper())
            elif isinstance(item, float):
                result.append(round(item, 2))
            else:
                result.append(str(item))
                
        self.data = result
        return self.data

    # Метод с обработкой ошибок
    def save_to_file(self, filename):
        """
        Сохранение данных в файл
        """
        try:
            with open(filename, 'w') as file:
                for item in self.data:
                    file.write(f"{item}\n")
            return True
        except Exception as e:
            print(f"Ошибка при сохранении: {str(e)}")
            return False

    # Метод с документацией
    def get_statistics(self):
        """
        Получение базовой статистики по данным
        
        Возвращает:
            dict: словарь со статистикой
        """
        stats = {
            'count': len(self.data),
            'types': {type(item).__name__: self.data.count(item) for item in self.data},
            'processed': self.processed
        }
        return stats