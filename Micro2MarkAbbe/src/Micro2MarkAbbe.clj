(ns Micro2MarkAbbe)

(defn not-elimination [not-prop]
  (if (and (list? not-prop)
           (= (first not-prop) 'not)
           (list? (second not-prop))
           (= (first (second not-prop)) 'not)) ;;End of conditions
    #{(second (second not-prop))} ;;Then statement
    #{}) ;;Else
  )

(defn and-elimination [and-prop]
  (if (and (list? and-prop)
           (= (first and-prop) 'and)
           (list? (second and-prop))
           (= (first (second and-prop)) 'and)) ;;End of conditions
    #{(second (second and-prop))} ;;Then statement
    #{}) ;;Else
  )

(defn -main
  [& args]
  (println (not-elimination '(not x))) ;;Output: #{}
  (println (not-elimination '(not (not a)))) ;;Output: #{a}
  (println (not-elimination '(not (not (and a b))))) ;;Output: #{(and a b)}
  (println (not-elimination '(not (not (not (not c)))))) ;;Output: #{(not (not c))}
  )