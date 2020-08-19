(defun plist-add (plist new)
  
  (cond ((car plist)
         (cond ((<= new (car plist)) (append (list new) plist))
               (t (append (list (car plist)) (plist-add (cdr plist) new)))))
        (t (list new))))

(defun list-sort-i (newlist unslist)

  (cond ((car unslist) (set (quote newlist) (plist-add newlist (car unslist)))
         (list-sort-i newlist (cdr unslist))) (t newlist)))
  
