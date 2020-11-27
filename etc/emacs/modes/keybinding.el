;;----------------------------------------------
;; gdb and compile binding minor mode definition

(defvar pond-keys-minor-mode-map (make-sparse-keymap) "pond-keys-minor-mode keymap.") 



(setq compile-command "make -f Makefile")
(setq compile-install-command "make -f Makefile install")
(setq gud-gdb-command-name "cuda-gdb --annotate=3 ")
(setq gud-gdb-command-name "gdb -i=mi ")

(defun gud-quit() (interactive) 
 (gud-call "quit")
 (sleep-for 0.5)
 (save-excursion
   (switch-by-str "*gud-") 
   (kill-buffer (current-buffer))
   (delete-window)
   )
 )

(defun shrink-compile-window()
  "shrink compile window, avoid compile window occupy 1/2 hight of whole window"
  (interactive)
  (setq compiled_buffer_name (buffer-name (current-buffer)))
  (switch-to-buffer-other-window "*compilation*")
  (if (< (/ (frame-height) 3.0) (window-height))
    (shrink-window (/ (window-height) 2)))
  (switch-to-buffer-other-window compiled_buffer_name)
  )
(defun du-onekey-compile ()
  "Save buffers and start compile"
  (interactive)
  (save-some-buffers t)
  (compile compile-command)
  (shrink-compile-window)
  )

(defun du-onekey-compile-install ()
  "Save buffers and start compile"
  (interactive)
  (save-some-buffers t)
  (compile compile-install-command)
  (shrink-compile-window)
  )

(defun gud-break-or-remove()
  "create a break point or remove it"
  (interactive)
  (save-excursion
	(if (eq (car (fringe-bitmaps-at-pos (point))) 'breakpoint)
		(gud-remove nil)
	  (gud-break nil))))
(defun gud-start() (interactive) (gud-call "start"))
(defun my-buffer-switch()
  "switch to most recently opened buffer"
  (interactive)
  (switch-to-buffer (other-buffer (current-buffer) t)))

(defun my-fullscreen ()
  (interactive)
  (x-send-client-message
    nil 0 nil "_NET_WM_STATE" 32
    '(2 "_NET_WM_STATE_FULLSCREEN" 0)
    )
  )
;;(if (display-graphic-p) (my-fullscreen))
;;switch to a buffer with part name matched prefix letters
(defun str-match (str1  str2)
  (interactive)
  (and (<= (string-width str1) (string-width str2))
		   (equal str1 (substring str2 0 (string-width str1) )))
  )
(defun buffer-name-by-str (bstr)
  (interactive)
  (setq bl (buffer-list))
  (setq ans nil)
  (while bl
         (setq bnn (buffer-name (setq bn (pop bl))))
         (if (str-match bstr bnn)
           (setq ans bnn)))
  ans
  )

(defun switch-by-str (bstr)
  (interactive) (switch-to-buffer-other-window (buffer-name-by-str bstr)))
(defun window-height-to (height)
  (interactive)
  (while (> (window-height) height) (shrink-window 1))
  (while (< (window-height) height) (shrink-window -1))
  )
(defun window-width-to-ratio(ratio)
  (interactive)
  (if (< (- (frame-width) 10) (window-width)) nil
    (while (> (/ (float (window-width)) (frame-width)) ratio) (shrink-window-horizontally 1))
    (while (< (/ (float (window-width)) (frame-width)) ratio) (shrink-window-horizontally -1))
    )
  )

(defun my-window-switch()
  "switch to most recently opened buffer"
  (interactive)
  (switch-to-buffer-other-window
   (buffer-name (other-buffer (current-buffer) t))))

(setq gdb-many-windows t)
(setq gdb-show-main t)
(setq gdb-use-separate-io-buffer t) 
;;"my gdb UI"
(defun gud-change-layout ()
  (interactive)
  (switch-by-str "*stack frames of") (window-height-to 5)
  (switch-by-str "*breakpoints of") (delete-window)
  (switch-by-str "*locals of")
  ;;(switch-by-str "*gud-") (window-height-to-ration 0.3)
  (switch-by-str "*gud-") 
  (window-width-to-ratio 0.7)
  (windmove-down) (window-width-to-ratio 0.7)
  )
(add-hook 'gdb-mode-hook 'gud-change-layout)

(defun gud-excu()
  (interactive)
  (if (str-match "*gud-" (buffer-name))
    ((lambda () (interactive)
       (comint-send-input)
       (my-window-switch ))
     )
    ((lambda () (interactive)
       (switch-by-str "*gud-")
       ))
    )
  )

(defun gud-mode-keys ()
  (define-key pond-keys-minor-mode-map (kbd "<prior>") 'comint-previous-input)
  (define-key pond-keys-minor-mode-map (kbd "<up>") 'comint-previous-input)
  (define-key pond-keys-minor-mode-map (kbd "<next>")  'comint-next-input)
  (define-key pond-keys-minor-mode-map (kbd "<down>")  'comint-next-input)
  ;...
  )
(add-hook 'gdb-mode-hook 'gud-mode-keys)
  
;; recent files
(require 'recentf)
(recentf-mode 1)
(setq recentf-max-menu-items 25)

(define-key pond-keys-minor-mode-map (kbd "M-b") 'gud-break)
(define-key pond-keys-minor-mode-map (kbd "M-B") 'gud-remove)
(define-key pond-keys-minor-mode-map (kbd "M-r") 'gud-go)
(define-key pond-keys-minor-mode-map (kbd "M-s") 'gud-start)
(define-key pond-keys-minor-mode-map (kbd "M-n") 'gud-next)
(define-key pond-keys-minor-mode-map (kbd "M-i") 'gud-step)
(define-key pond-keys-minor-mode-map (kbd "M-f") 'gud-finish)
(define-key pond-keys-minor-mode-map (kbd "M-u") 'gud-until)
(define-key pond-keys-minor-mode-map (kbd "M-p") 'gud-print)
(define-key pond-keys-minor-mode-map (kbd "M-P") 'gud-pstar)


(define-key pond-keys-minor-mode-map (kbd "M-2") 'save-buffer )
(define-key pond-keys-minor-mode-map [(f10)] 'save-buffer )

(define-key pond-keys-minor-mode-map (kbd "C-s") 'save-buffer)

(define-key pond-keys-minor-mode-map "\C-h" 'backward-delete-char-untabify)  




(define-key pond-keys-minor-mode-map "\C-x\ \C-r" 'recentf-open-files)


(define-key pond-keys-minor-mode-map (kbd "M-3") 'buffer-menu)
(define-key pond-keys-minor-mode-map (kbd "M-4") 'my-buffer-switch)

;;(define-key pond-keys-minor-mode-map (kbd "M-4") 'buffer-menu)


(winner-mode t)

(define-key pond-keys-minor-mode-map (kbd "M-[") 'winner-undo)

(define-key pond-keys-minor-mode-map (kbd "M-]") 'winner-redo)

(define-key pond-keys-minor-mode-map [(control k)] 'delete-window ) 

(define-key pond-keys-minor-mode-map [(control K)] 'kill-buffer-and-window)






(define-key pond-keys-minor-mode-map (kbd "M-h") 'windmove-left) 
(define-key pond-keys-minor-mode-map (kbd "M-j") 'windmove-down) 
(define-key pond-keys-minor-mode-map (kbd "M-k") 'windmove-up) 
(define-key pond-keys-minor-mode-map (kbd "M-l") 'windmove-right) 

(define-key pond-keys-minor-mode-map (kbd "M-e") 'next-error)
(define-key pond-keys-minor-mode-map (kbd "M-E") 'previous-error)


(define-key pond-keys-minor-mode-map (kbd "M-1") '(lambda ()
                          (interactive)
                          (call-interactively 'gdb)
                          (sleep-for 1.5)
                          (gud-change-layout) )
                )

(define-key pond-keys-minor-mode-map (kbd "M-!") 'gud-quit)

;;(define-key pond-keys-minor-mode-map (kbd "C-`") 'compile)

(define-key pond-keys-minor-mode-map (kbd "M-`") 'du-onekey-compile)

(define-key pond-keys-minor-mode-map (kbd "M-~") 'compile)
;;(define-key pond-keys-minor-mode-map (kbd "M-~") 'du-onekey-compile-install)

(define-key pond-keys-minor-mode-map  (kbd "M-0") 'my-fullscreen) 

(define-key pond-keys-minor-mode-map (kbd "M-RET") 'semantic-ia-fast-jump)
(define-key pond-keys-minor-mode-map (kbd "C-RET") 
			 (lambda () (interactive)
			   (if (ring-empty-p (oref semantic-mru-bookmark-ring ring))
				(error "Semantic Bookmark ring is empty"))
			   (let* ((ring (oref semantic-mru-bookmark-ring ring))
					(alist (semantic-mrub-ring-to-assoc-list ring))
					(first (cdr (car alist))))
				(if (semantic-equivalent-tag-p (oref first tag)
										 (semantic-current-tag))
				  (setq first (cdr (car (cdr alist)))))
				(semantic-mrub-switch-tags first)
				)
			   )
			 )

(define-minor-mode pond-keys-minor-mode
  "A minor mode for pond env."
  :init-value t :lighter "")
(pond-keys-minor-mode 1)
(provide 'keybinding)
