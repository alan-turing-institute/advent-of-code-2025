#lang racket

(module+ main

  (define-values (*ranges* *ids*)
    (with-input-from-file "input.txt" parse-input))

  ;; Part 1
  (length
   (filter (λ (id) (fresh? id *ranges*)) *ids*))

  ;; Part 2
  (for/sum ([r (in-list (make-i-list *ranges*))])
    (+ (- (cadr r) (car r)) 1))

  )



;; i1 is < i2 if the upper limit of i1 is less than the lower limit of i2 minus 1
(define (interval<? i1 i2)
  (< (cadr i1) (- (car i2) 1)))

;; An i-list is an ordered set of non-overlapping intervals. That is,
;; for i1, i2 any to successive interverals in the i-list, (interval<?
;; i1 i2) is #t

(define (make-i-list is)
  (for/fold ([ilist '()])
            ([i (in-list is)])
;    (println (format "Inserting ~a" i))
    (let ([new (insert i ilist)])
;      (println new)
      new)))

;; insert : interval? i-list? -> i-list?
;;
;; Insert the interval into an i-list.
(define (insert interval i-list)
  (insert1 '() i-list interval))

;; Take each car of xs and cons onto ys
(define (reverse-append xs ys)
  (if (null? xs)
      ys
      (reverse-append (cdr xs) (cons (car xs) ys))))

;; must have neither (interval<? i1 i2) nor (interval<? i2 i1)
(define (interval-merge i1 i2)
  (list (min (car i1) (car i2))
        (max (cadr i1) (cadr i2))))

;; The upper limit of ls is less than the lower limit of rs and of i
;; That is, the ls are "done" and the rs are "still to do"
;; The ls are in reverse order -- the car is the last one to be appended
(define (insert1 ls rs i)
  (if (null? rs)
      (reverse (cons i ls))
      (let ([r (car rs)])
        (if (interval<? r i)
            (insert1 (cons r ls) (cdr rs) i)
            (if (interval<? i r)
                ;; Add i and then put the ls back in order
                (reverse-append ls (cons i rs))
                (insert1 ls (cdr rs) (interval-merge i r)))))))


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

  ;; Part 2
  (for/sum ([r (in-list (make-i-list *ranges*))])
    (+ (- (cadr r) (car r)) 1))

  )
