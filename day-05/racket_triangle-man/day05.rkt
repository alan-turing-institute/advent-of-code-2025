#lang racket

(module+ main

  (define-values (*ranges* *ids*)
    (with-input-from-file "input.txt" parse-input))

  ;; Part 1
  (length
   (filter (λ (id) (fresh? id *ranges*)) *ids*))



  )

;; An i-list is an ordered set of non-overlapping intervals. Two
;; intervals overlap if the upper limit of one is greater than two
;; less than the lower limit of the other. 
;; 

;; insert : interval? i-list? -> i-list?
;;
;; Insert the interval into an i-list.
(define (insert interval i-list)
  
  )

(define (in-range? ingredient range)
  (and (>= ingredient (car range))
       (<= ingredient (cadr range))))

(define (fresh? ingredient ranges)
  (ormap (λ (rng) (in-range? ingredient rng)) ranges))

(define (parse-input)
  (let* ([input
          (string-split (port->string) "\n\n")]
         [ranges
          (map (λ (s) (map string->number (string-split s "-")))
               (string-split (car input) "\n"))]
         [ingredients
          (map string->number
               (string-split (cadr input) "\n"))])
    (values ranges ingredients))
  )

;; ----------------------------------------------------------------------

(module+ test

  (define *input* #<<EOF
3-5
10-14
16-20
12-18

1
5
8
11
17
32
EOF
    )

  (define-values (*ranges* *ids*)
    (with-input-from-string *input* parse-input))

  ;; Part 1
  (length
   (filter (λ (id) (fresh? id *ranges*)) *ids*))

  )
