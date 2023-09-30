(ns Micro2MarkAbbe)

(defn not-elimination [not-prop]
  (if (and (list? not-prop)
           (= (first not-prop) 'not)
           (list? (second not-prop))
           (= (first (second not-prop)) 'not)) ;;End of conditions
    #{(second (second not-prop))} ;;Then statement
    #{}) ;;Else
  )

;;Probably wont need (not using vectors)
(def not_expr1 [not not :p])
(def not_expr2 [not :q])

(defn -main
  [& args]
  (println (not-elimination '(not x))) ;;Output: #{}
  (println (not-elimination '(not (not a)))) ;;Output: #{a}
  (println (not-elimination '(not (not (and a b))))) ;;Output: #{(and a b)}
  (println (not-elimination '(not (not (not (not c)))))) ;;Output: #{(not (not c))}
  )