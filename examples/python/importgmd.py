import asyncio
import json
import sys
import websockets

async def main():
    uri = "ws://127.0.0.1:1313"
    file_path = "examples/example.gmd"

    try:
        with open(file_path, "r", encoding="utf-8") as f:
            gmd_content = f.read()
    except FileNotFoundError:
        print(f"Ошибка: файл '{file_path}' не найден.")
        sys.exit(1)
    except Exception as e:
        print(f"Ошибка при чтении файла: {e}")
        sys.exit(1)

    message = {
        "action": "IMPORT_GMD",
        "close": True,
        "gmd": gmd_content[:200]
    }

    try:
        async with websockets.connect(uri) as websocket:
            await websocket.send(json.dumps(message))
            print("Сообщение отправлено")

            response = await websocket.recv()
            print(f"Ответ сервера: {response}")

    except ConnectionRefusedError:
        print(f"Ошибка: не удалось подключиться к {uri}. Убедитесь, что сервер запущен.")
        sys.exit(1)
    except Exception as e:
        print(f"Ошибка при обмене данными: {e}")
        sys.exit(1)

if __name__ == "__main__":
    asyncio.run(main())