#lang racket

(require racket/list/grouping)


(define (propagate-beams beams splitter-rows)
  (for/fold ([n 0]
             [beams beams])
            ([splitters (in-list splitter-rows)])
    (for/list ([beam (in-list beams)])
      ())
    ))


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
  
  (with-input-from-string *input* parse-input)


  )
