#lang racket

(require "grid.rkt")

(module+ main
  
  (define *puzzle*
    (lists->grid
     (map string->list
          (with-input-from-file "input.txt" port->lines))))

  ;; Part 1
  (count-adjacent-rolls *puzzle*)

  ;; Part 2
  (count-all-removals *puzzle*)
  
  )

;; ----------------------------------------------------------------------

(define (print-grid g)
  (display
   (string-join 
    (map list->string (grid->lists g))
    "\n")))

(define (is-roll? c)
  (char=? c #\@))

(define (grid-refs g ps)
  (map (λ (p) (grid-ref g p)) ps))

(define (accessible-rolls g)
  (define (accessible pos v)
    (if (and (is-roll? v)
             (< (length (filter is-roll?
                                (grid-refs g (grid-8-adjacents g pos))))
                4))
        #\x
        v))
  (grid-indexed-map accessible g))

(define (count-adjacent-rolls g)
  (for*/sum ([r (in-range (grid-nrows g))]
             [c (in-range (grid-ncols g))])
    ;    (print (format "At ~a there are " (cons r c)))
    (let* ([here (cons r c)]
           [nadj
            (length (filter is-roll?
                            (grid-refs g (grid-8-adjacents g (cons r c)))))])
      ;      (println (format "~a adjacent" nadj))
      (if (and (is-roll? (grid-ref g here))
               (< nadj 4))
          1
          0))))

;; -> (values number? grid?)
(define (remove-accessible-rolls g)
  (define (update-cell pos v)
    (if (and (is-roll? v)
             (< (length (filter is-roll? (grid-refs g (grid-8-adjacents g pos)))) 4))
        #\x
        v))
  (let ([xed-grid (grid-indexed-map update-cell g)])
    (values (length (grid-indexes-of xed-grid #\x))
            (grid-map (λ (v) (if (eq? v #\x) #\. v)) xed-grid))
    ))

(define (count-all-removals g)
  (let loop ([total 0]
             [so-far g])
    (let-values ([(n new) (remove-accessible-rolls so-far)])
      (println n)
      (if (zero? n)
          total
          (loop  (+ total n) new)))))


;; ----------------------------------------------------------------------

(module+ test

  (define *input* #<<EOT
..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.
EOT
    )

  (define *puzzle*
    (lists->grid
     (map string->list
          (with-input-from-string *input* port->lines))))

  )
