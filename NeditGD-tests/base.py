from NeditGD import Editor, Object

editor = Editor.load_live_editor()

editor.add_object(Object(id = "text", x = 15, y = 15, text = "Hello World!"))

editor.save_changes()

# python NeditGD-tests/base.py