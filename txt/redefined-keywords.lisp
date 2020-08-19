;; ((lambda (!o ...) s ...) s ...)

(defun list (...)

  ;; (list s ...)
  ;; (list)

  ...)

(defun map (func lst)

  ;; (map s s)

  (append (list (eval (list func (car lst))))
          (cond ((cdr lst) (map func (cdr lst))))))

(defun let ((args quoted) ...)

  ;; (let ((!o s) ...) s ...)
  ;; (let ((!o)))

    (eval (list (list lambda (list (map car args)) ...) (map cdr args))))

(defun quote ((arg quoted))

  ;; (quote s)

  arg)
