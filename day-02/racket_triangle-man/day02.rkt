#lang racket

(module+ main

  (define *puzzle*
    (parse-input
     (string-trim (call-with-input-file "input.txt" port->string))))

  ;; Part 1
  (apply +
         (append-map (invalids-in-range is-invalid/part1?) *puzzle*))

  ;; Part 2
  (apply +
         (append-map (invalids-in-range is-invalid/part2?) *puzzle*))

  )



;; string? -> list-of pair?
(define (parse-input in)
  (map
   (λ (s) (map string->number (string-split s "-")))
   (string-split in ",")))

(define (is-invalid/part1? n)
  (let ([ndigits (ceiling (log n 10))])
    (and (even? ndigits)
         (let ([half10s (expt 10 (/ ndigits 2))])
           (let-values ([(p r) (quotient/remainder n half10s)])
             (= p r))))))

;; For small n!
(define (factors-of n)
  (filter (λ (g) (zero? (modulo n g)))
          (range 1 n)))

;; Is n the same as its first g digits (of h) replicated?
(define (replicated-digits? g h n)
  (let* ([pow (expt 10 g)]
         [digits (remainder n pow)]
         [replicated
          (for/sum ([i (in-range (quotient h g))])
            (* digits (expt pow i)))])
    (= replicated n)))

(define (is-invalid/part2? n)
  (let* ([ndigits (ceiling (log n 10))]
         [factors (factors-of ndigits)])
    (for/or ([g factors])
      (replicated-digits? g ndigits n))))

(define ((invalids-in-range is-invalid?) r)
  (filter is-invalid? (range (car r) (+ 1 (cadr r)))))



(module+ test

  (define *input* "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124")

  (define *puzzle*
    (parse-input *input*))

  ;; Part 2
  (apply +
         (append-map (invalids-in-range is-invalid/part2?) *puzzle*))

  
  )
