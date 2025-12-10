#lang racket

(module+ main


  (define-values (*rows* *ops*)
    (with-input-from-file "input.txt" parse-input))

  ;; Part 1
  (apply +
         (map symbol-apply *ops* (transpose *rows*)))
  
 ;; Part 2
  (define *rows2*
    (map list->string
         (transpose
          (with-input-from-file "input.txt" parse-input2))))

  (apply +
         (map symbol-apply *ops*
              (mmap (compose string->number string-trim)
                    (list-split *rows2* (λ (s) (regexp-match #rx"^ +$" s))))))


  )

;; ----------------------------------------------------------------------

(define (symbol-apply op xs)
  (eval `(apply ,op (quote ,xs))))

;; I've never really understood this!
(define (transpose xss)
  (apply map list xss))


;; Split a list on elements for which pred? is #t
(define (list-split xs pred?)
  (for/fold ([all '()]
             [this '()]
             #:result (reverse (cons (reverse this) all))) 
            ([x (in-list xs)])
    (if (pred? x)
        (values (cons (reverse this) all) '())
        (values all (cons x this)))))

;; map over the elements of the elements of xss
(define (mmap proc xss)
  (map (λ (xs) (map proc xs)) xss))

(define (parse-input)
  (let* ([lines (map string-split (port->lines))]
         [rows  (mmap string->number (drop-right lines 1))]
         [ops   (map string->symbol (car (take-right lines 1)))])
    (values rows ops)))

(define (parse-input2)
  (let ([lines (port->lines)])
    (map string->list (drop-right lines 1))))



;; ----------------------------------------------------------------------

(module+ test

  (define *input* #<<EOF
123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  
EOF
    )

  (define-values (*rows* *ops*)
    (with-input-from-string *input* parse-input))

  ;; Part 1
  (apply +
         (map symbol-apply *ops* (transpose *rows*)))
  
  ;; Part 2
  (define *rows2*
    (map list->string
         (transpose
          (with-input-from-string *input* parse-input2))))

  (apply +
         (map symbol-apply *ops*
              (mmap (compose string->number string-trim)
                    (list-split *rows2* (λ (s) (regexp-match #rx"^ +$" s))))))


  
  )
