for file in $(ls ./map/ -1); do
    echo -e "$file: \t\t $(./agent $file)";
done
