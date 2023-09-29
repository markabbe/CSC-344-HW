(ns Micro2MarkAbbe)

(def randVar 10)

(defn not-elimination [not-expression]
  (cond
    (not (and (vector? not-expression) (= (first not-expression) 'not) (= (count not-expression) 2)))
    (hash-set)

    :else
    (hash-set (second not-expression))
    ))

(def not_expr1 [:not :p])
(def not_expr2 [:not :q])
(def invalid_expr [:and :p :q])

(defn -main
  [& args]
  (println (not-elimination not_expr1))
  (println (not-elimination not_expr2))
  (println (not-elimination invalid_expr)))