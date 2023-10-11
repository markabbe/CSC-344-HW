(ns Micro2MarkAbbe
  (:require [clojure.set :as set]))

(defn not-elimination [not-prop]
  (if (and (list? not-prop)
           (= (first not-prop) 'not)
           (list? (second not-prop))
           (= (first (second not-prop)) 'not))
    [#{(second (second not-prop))} "not-elimination"]
    [#{} ""]))

(defn and-elimination [and-prop]
  (if (and (list? and-prop)
           (= (first and-prop) 'and))
    [(set (rest and-prop)) "and-elimination"]
    [#{} ""]))

(defn modus-ponens [if-prop kb]
  (if (and (list? if-prop)
           (= (first if-prop) 'if)
           (contains? kb (second if-prop)))
    [#{(nth if-prop 2)} "modus ponens"]
    [#{} ""]))

(defn modus-tollens [if-prop kb]
  (if (and (list? if-prop)
           (= (first if-prop) 'if)
           (contains? kb (list 'not (nth if-prop 2))))
    [#{(list 'not (second if-prop))} "modus tollens"]
    [#{} ""]))

(defn elim-step [prop kb]
  (if (list? prop)
    (cond
      (= (first prop) 'not) (not-elimination prop)
      (= (first prop) 'and) (and-elimination prop)
      (= (first prop) 'if)
      (cond
        (contains? kb (second prop)) (modus-ponens prop kb)
        (contains? kb (list 'not (nth prop 2))) (modus-tollens prop kb)
        :else [#{} ""])
      :else [#{} ""])
    [#{} ""]))

(defn fwd-infer [prop kb]
  (loop [current-kb (conj kb prop)
         reasons    []]
    (let [[inferred new-reasons]
          (reduce (fn [[infs reas] p]
                    (let [[res reason] (elim-step p current-kb)]
                      (if (not-empty res)
                        [(set/union infs res)
                         (conj reas (str "Because: " (pr-str p)
                                         "\nI derived: " (pr-str res)
                                         "\nby " reason))]
                        [infs reas])))
                  [#{}
                   []]
                  current-kb)]
      (if (set/subset? inferred current-kb)
        [current-kb (distinct reasons)]
        (recur (set/union current-kb inferred) (concat reasons new-reasons))))))


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
  (println (modus-ponens '(if a b) '#{b, t, u, i, r, a})) ;;Output: #{b}
  (println (modus-ponens '(if a sdgdgdsf) '#{b, t, u, i, r, a})) ;;Output: #{sdgdgdsf}
  (println)
  (println "Modus-Tollens:")
  (println (modus-tollens '(if a b) '#{(not b)}))       ;;Output: #{(not a)}
  (println (modus-tollens '(if a b) '#{b}))            ;;Output: #{}
  (println (modus-tollens '(if (not a) b) '#{(not b)})) ;;Output: #{(not (not a))}
  (println (modus-tollens '(if a b) '#{c, d, e}))      ;;Output: #{}
  (println)
  (println "Fwd-Infer:")

    ;; Test 1
    (let [[result reasons] (fwd-infer '(if a b) '#{(not b)})]
      (println "Given (if a b) and #{(not b)}:")
      (doseq [r reasons]
        (println r))
      (println "Derived:" result)
      (println))

    ;; Test 2
    (let [[result reasons] (fwd-infer 'a '#{(if a b) (if b c)})]
      (println "Given 'a' and #{(if a b) (if b c)}:")
      (doseq [r reasons]
        (println r))
      (println "Derived:" result)
      (println))

    ;; Test 3
    (let [[result reasons] (fwd-infer '(and (not (not (if a b))) a) '#{})]
      (println "Given '(and (not (not (if a b))) a)' and #{}:")
      (doseq [r reasons]
        (println r))
      (println "Derived:" result)
      (println))
    )