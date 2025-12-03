#lang racket

;; Eg (turn 'right 50)
(struct turn (dir amt) #:transparent)

(define (parse-input)
  (for/list ([line (in-lines)])
    (let ([dir (substring line 0 1)]
          [amt (substring line 1)])
      (turn
       (match dir
         ["R" 'right]
         ["L" 'left])
       (string->number amt)))))

(struct state (dial count0) #:transparent)

;; Part 1
(define (run1 ins current-state)
  (match-let ([(state dial count0) current-state])
    (let ([new-dial
           (if (eq? (turn-dir ins) 'right)
               (modulo (+ dial (turn-amt ins)) 100)
               (modulo (- dial (turn-amt ins)) 100))])
      (state
       new-dial
       (if (zero? new-dial)
           (+ count0 1)
           count0)))))

;; Part 2
(define (turn-right start amt)
  (quotient/remainder (+ start amt) 100))

(define (turn-left start amt)
  (let ([start (modulo (- 100 start) 100)])
    (let-values ([(p r) (turn-right start amt)])
      (values p (modulo (- 100 r) 100)))))

(define (run2 ins current-state)
  (match-let ([(state dial count0) current-state]
              [(turn dir amt)      ins])
    (let-values ([(p r)
                  (if (eq? dir 'right)
                      (turn-right dial amt)
                      (turn-left dial amt))])
      (state r (+ count0 p)))))


(module+ main
  (define *input*
    (with-input-from-file "input.txt" parse-input))

  ;; Part 1
  (foldl run1 (state 50 0) *input*)

  ;; Part 2
  (foldl run2 (state 50 0) *input*)
    
  )


(module+ test

  (define *test* #<<EOS
L68
L30
R48
L5
R60
L55
L1
L99
R14
L82
EOS
    )

  (define *input*
    (with-input-from-string *test* parse-input))

  ;; Part 1
  (foldl run1 (state 50 0) *input*)

  ;; Part 2
  (foldl run2 (state 50 0) *input*)
  
  )
