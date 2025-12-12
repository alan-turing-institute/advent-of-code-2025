#lang racket

(require racket/list/grouping)

(module+ main

  (define *puzzle*
    (with-input-from-file "input.txt" parse-input))

  ;; Part 1
  (propagate-beams (hash (caar *puzzle*) 1) (cdr *puzzle*) (λ (c1) (λ (c2) 1)))

  ;; Part 2
  (define-values (_ *beams*)
    (propagate-beams (hash (caar *puzzle*) 1) (cdr *puzzle*) (λ (c1) (λ (c2) (+ c1 c2)))))

  (apply + (hash-values *beams*))


  )


(define (add-beam ht loc updater)
  (if (hash-has-key? ht loc)
      (hash-update ht loc updater)
      (hash-set ht loc (updater 0))))


;; propagate-beams needs a beam-updater procedure.
;; beam-updater : number? -> number? -> number?
;;
;; beam-updater takes the counts of two beams which have merged and
;; produces the new count for the merged beam. For part 1, that is
;; always 1; for part 2, it is the sum of the individual beam counts.
(define (propagate-beams beams splitter-rows beam-updater)
  (for/fold ([n 0]
             [beams beams])
            ([splitters (in-list splitter-rows)])
    (let-values ([(m split-beams)
                  (for/fold ([n 0]
                             [split-beams (make-immutable-hash)])
                            ([(loc count) (in-hash beams)])
                    (if (member loc splitters)
                        (values
                         (+ n 1)
                         (add-beam
                          (add-beam split-beams (+ loc 1) (beam-updater count))
                          (- loc 1) (beam-updater count)))
                        (values
                         n
                         (add-beam split-beams loc (beam-updater count)))))])
      (println split-beams)
      (values
       (+ n m)
       split-beams))))


(define (find-splitters xs)
  (let ([row (string->list (car xs))])
    (indexes-where row (λ (c) (or (char=? c #\^) (char=? c #\S))))))

(define (parse-input)
  (let* ([lines (port->lines)])
    (map find-splitters (windows 2 2 lines)))

  )

;; ----------------------------------------------------------------------

(module+ test

  (define *input* #<<EOF
.......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............
EOF
    )

  (define *puzzle*
    (with-input-from-string *input* parse-input))

  ;; Part 1
  (propagate-beams
   (hash (caar *puzzle*) 1)      ; Starting column
   (cdr *puzzle*)                ; List of locations of splitters on each row
   (λ (count1) (λ (count2) 1)))  ; When combining beams, just maintain one

  ;; Part 2
  (define-values (_ beams)
    (propagate-beams
     (hash (caar *puzzle*) 1)
     (cdr *puzzle*)
     (λ (count1) (λ (count2) (+ count1 count2)))))
  
  (apply + (hash-values beams))

  )
