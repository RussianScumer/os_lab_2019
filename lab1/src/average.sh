#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: $0 <input_file>"
  exit 1
fi

input_file="$1"

if [ ! -f "$input_file" ]; then
  echo "File $input_file not found."
  exit 1
fi

# Считываем числа из файла и вычисляем сумму и количество чисел
sum=0
count=0

while IFS= read -r line; do
  sum=$((sum + line))
  count=$((count + 1))
done < "$input_file"

# Вычисляем среднее арифметическое
average=$(bc -l <<< "scale=2; $sum / $count")

# Выводим результат
echo "Количество чисел: $count"
echo "Среднее арифметическое: $average"
