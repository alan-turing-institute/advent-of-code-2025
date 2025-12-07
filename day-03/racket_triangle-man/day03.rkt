#lang racket

(module+ main
  
  (define *puzzle*
    (map (λ (bank) (map char->digit (string->list bank)))
         (with-input-from-file "input.txt" port->lines)))
  
  ;; Part 1
  (apply + (map (max-joltage 2) *puzzle*))
  
  ;; Part 2
  (apply + (map (max-joltage 12) *puzzle*))

  )



;; ----------------------------------------------------------------------

(define (char->digit c)
  (- (char->integer c) (char->integer #\0)))

;; Return the sublist starting from the first occurence of the max of
;; the xs, not including the last n elements.
(define (find-first-max n xs)
  (define (find-first-max/recur m ms xs)
    (if (zero? m)
        ms
        (if (> (car xs) (car ms))
            (find-first-max/recur (- m 1) xs (cdr xs))
            (find-first-max/recur (- m 1) ms (cdr xs)))))
  (find-first-max/recur (- (length xs) n)'(0) xs))

(define ((max-joltage N) bank)
  (let-values
      ([(joltage _)
        (for/fold ([acc 0]
                   [xs bank])
                  ([n (in-inclusive-range (- N 1) 0 -1)])
          ;; (println (format "acc: ~a" acc))
          ;; (println (format "xs:  ~a" xs))
          ;; (println (format "n:   ~a" n))
          ;; (println "-----------")
          (let ([ms (find-first-max n xs)])
            (values (+ (car ms) (* acc 10))
                    (cdr ms))))])
    joltage))


;; ----------------------------------------------------------------------

(module+ test

  (define *input* #<<EOT
987654321111111
811111111111119
234234234234278
818181911112111
EOT
    )

  (define *puzzle*
    (map (λ (bank) (map char->digit (string->list bank)))
         (with-input-from-string *input* port->lines)))

  ;; Part 1
  (apply + (map (max-joltage 2) *puzzle*))

  ;; Part 2
  (apply + (map (max-joltage 12) *puzzle*))

  
  )

