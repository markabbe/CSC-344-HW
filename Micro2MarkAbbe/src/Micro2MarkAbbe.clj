(ns Micro2MarkAbbe)

(defn not-elimination [not-prop]
  (if (and (list? not-prop)
           (= (first not-prop) 'not)
           (list? (second not-prop))
           (= (first (second not-prop)) 'not)) ;;End of conditions
    #{(second (second not-prop))} ;;Then statement (return)
    #{}) ;;Else
  )

(defn and-elimination [and-prop]
  (cond
    (and (list? and-prop)
         (= (first and-prop) 'and)
         (list? (second and-prop))) ;;End of first conditionals conditions
    #{(str (second and-prop) " " (nth and-prop 2))} ;;Then statement (return)

    (and (list? and-prop)
         (= (first and-prop) 'and)
         (>= (count and-prop) 3)) ;;End of second conditionals conditions
    #{(second and-prop) (nth and-prop 2)} ;;Then statement (return)
    :else #{})
  )

(defn modus-ponens [if-prop kb]
  (if (and (list? if-prop)
           (= (first if-prop) 'if)
           (contains? kb (second if-prop)))
    #{(nth if-prop 2)}
    #{}))

(defn modus-tollens [if-prop kb]
  (if (and (list? if-prop)
           (= (first if-prop) 'if)
           (contains? kb (list 'not (nth if-prop 2))))
    #{(list 'not (second if-prop))}
    #{}))

(defn -main
  [& args]
  (println"Not Elim:")
  (println (not-elimination '(not x))) ;;Output: #{}
  (println (not-elimination '(not (not a)))) ;;Output: #{a}
  (println (not-elimination '(not (not (and a b))))) ;;Output: #{(and a b)}
  (println (not-elimination '(not (not (not (not c)))))) ;;Output: #{(not (not c))}
  (println)
  (println "And Elim:")
  (println (and-elimination '(and ( a b ) a))) ;;Output: #{(a b) + a}
  (println (and-elimination '(and a b))) ;;Output: #{a b}
  (println (and-elimination '(and (and a b) c))) ;;Output: #{(and a b) c}
  (println (and-elimination '(not a))) ;;Output: #{}
  (println (and-elimination '(and a))) ;;Output: #{}
  (println (and-elimination '(and x y))) ;;Output: #{x y}
  (println (and-elimination '(not (and a b)))) ;;Output: #{}
  (println)
  (println "Modus-Ponens:")
  (println (modus-ponens '(if a b) '#{a})) ;;Output: #{b}
  (println (modus-ponens '(if a b) '#{b})) ;;Output: #{}
  (println (modus-ponens '(if (not a) b) '#{(not a)})) ;;Output: #{b}
  (println (modus-ponens '(if a b) '#{c, d, e})) ;;Output: #{}
  (println)
  (println "Modus-Tollens:")
  (println (modus-tollens '(if a b) '#{(not b)}))       ;;Output: #{(not a)}
  (println (modus-tollens '(if a b) '#{b}))            ;;Output: #{}
  (println (modus-tollens '(if (not a) b) '#{(not b)})) ;;Output: #{(not (not a))}
  (println (modus-tollens '(if a b) '#{c, d, e}))      ;;Output: #{}
  )