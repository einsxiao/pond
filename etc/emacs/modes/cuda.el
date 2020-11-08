;; cuda mode settings
;;(autoload 'cuda-mode "cuda-mode.el")
;(add-to-list 'auto-mode-alist '("\\.cu\\'" . c++-mode))
;(add-to-list 'auto-mode-alist '("\\.th\\'" . c++-mode))
;(add-to-list 'auto-mode-alist '("\\.h\\'" . c++-mode))
;(add-to-list 'auto-mode-alist '("\\.c\\'" . c++-mode))
;(add-to-list 'auto-mode-alist '("\\.cpp\\'" . c++-mode))
;;c++ types
(font-lock-add-keywords 'c++-mode '(("\\<\\(static_assert\\|assert\\|ensure\\)\\>" . font-lock-warning-face)))
(font-lock-add-keywords 'c++-mode '(("\\<s?size_t\\>" . 'font-lock-type-face)))
;;cuda types
(font-lock-add-keywords 'c++-mode '(("\\<__\\(device\\|global\\|shared\\|host\\|constant\\)__\\>" . 'font-lock-keyword-face)))
(font-lock-add-keywords 'c++-mode '(("\\<_n?\\(b\\|t\\)\\(x\\|y\\|z\\)\\>" . 'cuda-variable-face)))
(font-lock-add-keywords 'c++-mode '(("\\<_\\(vi\\|mx\\|my\\)\\>" . 'cuda-variable-face)))
(font-lock-add-keywords 'c++-mode '(("\\<\\(Real\\|Complex\\|Int\\|real\\|List\\|FunctionKey\\)\\>" . 'font-lock-type-face)))
(font-lock-add-keywords 'c++-mode '(("\\<\\(DeclareModule\\|DeclareFunction\\|ModuleInitialize\\|ModuleFinalize\\|DeclareFunction\\|DefineFunction\\|_Erroring\\|_Warning\\|Warning\\|Erroring\\|ReturnNull\\|WarnReturn\\|Null\\)\\>" . 'font-lock-keyword-face)))
(font-lock-add-keywords 'c++-mode '(("\\<\\(ThrowError\\|ReturnNumber\\|ReturnSymbol\\|ReturnString\\|ReturnNormal\\|ReturnHold\\|ReturnError\\|Return\\)\\>" . 'font-lock-keyword-face)))
(font-lock-add-keywords 'c++-mode '(("\\<\\(Argv\\|EvaKernel\\)\\>" . 'cuda-variable-face)))
(font-lock-add-keywords 'c++-mode '(("\\<\\(CheckShould.*\\)\\>" . 'cuda-variable-face)))

(add-hook 'c++-mode-hook
          (lambda ()
            (c-set-offset 'arglist-intro '+)
            (c-set-offset 'arglist-close 0)))
(provide 'cuda)




