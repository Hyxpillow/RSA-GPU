import os

# 生成 128 字节的随机数据
def generate_test_data(length=128):
    return os.urandom(length)  # 使用系统级别的随机字节生成

# 打印数据为十六进制格式
def print_hex(data):
    hex_str = ' '.join(f'{byte:02X}' for byte in data)  # 转换为十六进制字符串
    print(hex_str)

# 将数据写入二进制文件
def write_to_file(filename, data):
    with open(filename, 'wb') as f:
        f.write(data)  # 写入二进制文件
    print(f"测试数据已保存到文件: {filename}")

# 主程序
if __name__ == "__main__":
    test_data = generate_test_data()  # 生成 128 字节数据
    print("生成的 128 字节测试数据：")
    print_hex(test_data)  # 打印为十六进制格式

    write_to_file("test_data.bin", test_data)  # 写入二进制文件