for i in Logs/*.gv; do
    dot -Tjpg "$i" -O
done
