for i in Logs/*.gv; do
    dot -Tpng "$i" -O
done
