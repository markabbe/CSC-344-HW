(ns Micro2MarkAbbe)

(defn not-elimination [not-expression]
  (if (and (= (first not-expression) not) (= (count not-expression) 2))
    (hash-set)
    (if (and (vector? not-expression) (= (second not-expression) not) (= (first not-expression) not)
           (= (count not-expression) 3))
      (second not-expression)))


    ;:else
    ;(hash-set (second not-expression))
    ;)
  )

(def not_expr1 [not not :p])
(def not_expr2 [not :q])
(def invalid_expr [not :p :q])
(def nonVecExpr (not 1))

(defn -main
  [& args]
  (println (not-elimination not_expr1))
  (println (count not_expr1))
  (println (first not_expr1))
  (println)
  (println (not-elimination not_expr2))
  (println (count not_expr2))
  (println (first not_expr2))
  (println)
  (println (not-elimination invalid_expr))
  (println (count invalid_expr))
  (println (first invalid_expr))
  (println)
  (println (not-elimination '(not 1)))
  (println)
  (println (vector? []))
  (println (vector? ()))
  (println)
  (if   ( = 2 2 )
    (println "Equal")
    (println "Not Equal")))