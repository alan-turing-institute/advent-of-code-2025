#lang racket


;; A position is a list of three numbers
(define (dist2 p₁ p₂)
  (+
   (sqr (- (car p₂) (car p₁)))
   (sqr (- (cadr p₂) (cadr p₁)))
   (sqr (- (caddr p₂) (caddr p₁)))))

;; -> vector-of list?
(define (parse-input)
  (for/vector ([line (in-lines)])
    (map string->number (string-split line ","))))

;; Returns a sorted matrix
(define (make-distance-matrix locv)
  (let ([distances 
         (for*/list ([i (in-range 1 (vector-length locv))]
                     [j (in-range i)])
           (cons (cons i j) (dist2 (vector-ref locv i) (vector-ref locv j))))])
    (sort distances < #:key cdr)))


;; -------------------------------------------------------------------------------

(module+ test

  (define *input* #<<EOF
162,817,812
57,618,57
906,360,560
592,479,940
352,342,300
466,668,158
542,29,236
431,825,988
739,650,466
52,470,668
216,146,977
819,987,18
117,168,530
805,96,715
346,949,466
970,615,88
941,993,340
862,61,35
984,92,344
425,690,689
EOF
    )

  (define *locv*
    (with-input-from-string *input* parse-input))

  (define *dists*
    (make-distance-matrix *locv*))
  
  )

