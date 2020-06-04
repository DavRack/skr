import parserClasses
import unittest

class testParser(unittest.TestCase):

    def test_keyboardPath(self):
        data = [
        "KeyboardPath->/dev/input/event3",
        ]

         
        parsedData =[
        "NewLayer 0",
        "KeyboardPath /dev/input/event3",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "KeyboardPath   -> /dev/input/event3",
        ]

         
        parsedData =[
        "NewLayer 0",
        "KeyboardPath /dev/input/event3",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "KeyboardPath=/dev/input/event3",
        ]

         
        parsedData =[
        "NewLayer 0",
        "FAIL to parse line 1",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "KeyboardPath=>/dev/input/event3",
        ]

         
        parsedData =[
        "NewLayer 0",
        "FAIL to parse line 1",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "KeyboardPath<->/dev/input/event3",
        ]

         
        parsedData =[
        "NewLayer 0",
        "FAIL to parse line 1",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

    def test_newLayer(self):
        data = [
        "NewLayer->CapsLock"
        ]

        parsedData = [
        "NewLayer 0",
        "NewLayer 58",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "NewLayer -> CapsLock"
        ]

        parsedData = [
        "NewLayer 0",
        "NewLayer 58",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "NewLayer -> KeyCode(44)"
        ]

        parsedData = [
        "NewLayer 0",
        "NewLayer 44",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "TAB -> ESC",
        'NewLayer -> "ñalkjsdf"'
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 15 1 -1 0",
        "FAIL to parse line 2",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "TAB -> ESC",
        'NewLayer -> '
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 15 1 -1 0",
        "FAIL to parse line 2",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

    def test_remap(self):
        data = [
        "TAB -> ESC",
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 15 1 -1 0",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        "keyCode(15)->KeyCode(44)",
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 15 44 -1 0",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'tab->"a d"',
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 15 30 1 0.01",
        "KeyRemap 15 30 0 0.01",
        "KeyRemap 15 57 1 0.01",
        "KeyRemap 15 57 0 0.01",
        "KeyRemap 15 32 1 0.01",
        "KeyRemap 15 32 0 0.01",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'tab->script=htop&',
        ]

        parsedData = [
        "NewLayer 0",
        "Script 15 1 0 htop&",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'tab->script()=htop&',
        ]

        parsedData = [
        "NewLayer 0",
        "Script 15 1 0 htop&",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'tab->script(wait:3.4)=htop&',
        ]

        parsedData = [
        "NewLayer 0",
        "Script 15 1 3.4 htop&",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'tab->script(wait:3.4, keystate:release)=htop&',
        ]

        parsedData = [
        "NewLayer 0",
        "Script 15 0 3.4 htop&",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

    def test_macro(self):
        data = [
        'tab => a',
        '    => a',
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 15 30 1 0.01",
        "KeyRemap 15 30 0 0.01",
        "KeyRemap 15 30 1 0.01",
        "KeyRemap 15 30 0 0.01",
        ]
        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'tab => script=sudo htop&',
        '    => "a e"',
        '    => a',
        ]

        parsedData = [
        "NewLayer 0",
        "Script 15 1 0.01 sudo htop&",
        "KeyRemap 15 30 1 0.01",
        "KeyRemap 15 30 0 0.01",
        "KeyRemap 15 57 1 0.01",
        "KeyRemap 15 57 0 0.01",
        "KeyRemap 15 18 1 0.01",
        "KeyRemap 15 18 0 0.01",
        "KeyRemap 15 30 1 0.01",
        "KeyRemap 15 30 0 0.01",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        '    => script=sudo htop&',
        ]

        parsedData = [
        "NewLayer 0",
        "FAIL to parse line 1"
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

    def test_swap(self):
        data = [
        'meta <-> alt',
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 125 56 -1 0",
        "KeyRemap 56 125 -1 0",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'KeyCode(125) <-> KeyCode(56)',
        ]

        parsedData = [
        "NewLayer 0",
        "KeyRemap 125 56 -1 0",
        "KeyRemap 56 125 -1 0",
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'meta <->',
        ]

        parsedData = [
        "NewLayer 0",
        "FAIL to parse line 1"
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

        data = [
        'meta <-> "test"',
        ]

        parsedData = [
        "NewLayer 0",
        "FAIL to parse line 1"
        ]

        self.assertEqual(parsedData,parserClasses.parse(data))

if __name__ == '__main__':
    unittest.main()

