(set (quote *colours*) (quote ((0 0 0) (0 0 1) (1 0 0) (1 0 1)
                               (0 1 0) (0 1 1) (1 1 0) (1 1 1))))

(set (quote *colours*) (number-to-bin (+ (* 0 1) (* 4 8) (* 1 16) (* 5 24) (* 2 32) (* 6 40) (* 3 48) (* 7 56)) 64))

(set (quote *colours*) (list 0 4 1 5 2 6 3 7))


(defun iter (i n fun)

  (eval (list fun i))  
  (cond ((< i n) (iter (+ i 1) n fun))))

(defun find-node (list index)

  (cond ((> index 0) (find-node (cdr list (- index 1)))) (t (car list))))

(defun make-col (col val)

  (+ (* (* (/ col 4) val) 65536)
     (* (* (/ (% col 4) 2) val) 256)
     (* (% col 2) val)))
  
(defun unpack-rgb (x y)

  (let ((pixel-mask (bin-to-number (index *mask* 1 (+ (* (% (/ y 64) 4) 16384)
                                                      (* (% y 8) 2048)
                                                      (* (% (/ y 8) 8) 256)
                                                      x))))
        (pixel-attr (% (bin-to-number (index *attr* 8 (+ (* y 32) (/ x 8)))) 128))
        (pixel-int (/ pixel-attr 64)))

    
    (set (quote pixel-attr) (% (/ (% pixel-attr 64) (+ 1 (* pixel-mask 7))) 8))

    (make-col (find-node *colours* pixel-attr) (+ (* pixel-int 63) 192))))

(defun write-pixel (i)
  
  (write *fd-dst* (number-to-bin (unpack-rgb (% i 256) (/ i 256))  32)))


(set (quote *fd-src* (open (quote /tmp/1.scr) (quote r))))
(set (quote *fd-dst* (open (quote /tmp/2.raw) (quote w))))
(set (quote *mask*) (read *fd-src* 6144))
(set (quote *attr*) (read *fd-src* 768))
(close *fd-src*)
(iter 0 (* 256 192) write-pixel)
(close *fd-dst*)
