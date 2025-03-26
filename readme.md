# 校园导航系统 - 基于QT和C++实现

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![QT Version](https://img.shields.io/badge/Qt-5.15%2B-green.svg)](https://www.qt.io/)
[![GitHub stars](https://img.shields.io/github/stars/wang11222004/Campus-Navigation-Systrm-Based-on-QT-and-C-.svg?style=social)](https://github.com/wang11222004/Campus-Navigation-Systrm-Based-on-QT-and-C-)

## 🚀 项目概述
基于QT框架开发的校园导航系统，提供最优路径规划、地点搜索、地图交互等功能。适用于大型校园环境，帮助用户快速找到目的地。

## ✨ 核心功能
- **路径规划算法**
  - Dijkstra最短路径算法
  - A* 启发式搜索算法
  - 多路径选择（最短距离/最少时间）
  
- **交互功能**
  - 地图缩放拖拽
  - 地点关键字搜索
  - 路径动态显示
  - 导航步骤提示

- **数据管理**
  - CSV数据导入/导出
  - 地点信息编辑
  - 实时路径权重调整

## 🛠️ 开发环境
- **编译环境**: Qt 5.15+ (MSVC 2019 或 MinGW 64-bit)
- **依赖库**: 
  - Qt Charts (数据可视化)
  - C++17 标准库
- **推荐IDE**: Qt Creator 4.14+

## 📦 安装指南
### Windows 用户
1. 克隆仓库：
   ```bash
   git clone https://github.com/wang11222004/Campus-Navigation-System-Based-on-QT-and-C-.git
   ```
2. 使用Qt Creator打开 `CampusNavigation.pro` 文件
3. 配置构建套件（建议使用Release模式）
4. 编译并运行

### Linux 用户
```bash
sudo apt install qt5-default qt5-qmake
qmake CampusNavigation.pro
make && ./CampusNavigation
```

## 📂 项目结构
```text
Campus-Navigation-System/
├── data/                   # 地图数据文件
│   ├── campus_map.csv      # 节点连接数据
│   └── landmarks.csv       # 地点详细信息
├── src/                    # 源代码
│   ├── algorithm/         # 路径算法实现
│   ├── ui/                # QT界面设计
│   ├── mapwidget/          # 自定义地图组件
│   └── main.cpp
├── resources/              # 图形资源
│   ├── icons/             # 系统图标
│   └── styles/            # QSS样式表
└── CampusNavigation.pro    # QT项目文件
```

## 🗺️ 数据格式说明
### 节点文件示例 (campus_map.csv)
```csv
StartID,EndID,Distance,TimeWeight
A1,B2,150,2
B2,C3,200,3
```

### 地点文件示例 (landmarks.csv)
```csv
ID,Name,Description,PositionX,PositionY
A1,主校门,学校正门入口,125,340
B2,图书馆,5层建筑，藏书50万册,280,510
```

## 🤝 贡献指南
欢迎通过以下方式参与贡献：
1. Fork 本仓库
2. 创建特性分支 (`git checkout -b feature/新功能`)
3. 提交修改 (`git commit -am '添加新功能'`)
4. 推送分支 (`git push origin feature/新功能`)
5. 提交Pull Request

请确保代码符合 [Google C++代码规范](https://google.github.io/styleguide/cppguide.html)

## 📜 开源协议
本项目采用 MIT 许可证 - 详见 [LICENSE](LICENSE) 文件

---

> 🌟 提示：建议添加`docs`目录存放设计文档，使用Doxygen生成API文档：  
> `doxygen Doxyfile`
