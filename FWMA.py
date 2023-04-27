import matplotlib.pyplot as plt
import numpy as np

# 这个根据实际调整，因为队列没有填满的时候，也会继续计算的
FWMA_LEN = 5
'''FIFO长度'''

def FWMA_push(FWMA_t: dict, dat: float):
    '''
        往FWMA中添加数据
    '''

    FIFO: list = FWMA_t['fifo']
    '''队列'''

    x: list = FWMA_t['len']
    '''当前队列已经有多少个数据'''

    pos: int = FWMA_t['pos']
    '''当前元素的位置'''

    if (x < FWMA_LEN):
        x = x + 1

    # 弹出旧数据
    # FIFO[1:] = FIFO[:-1]
    # FIFO[0] = dat

    if (pos < x):
        FIFO[pos] = dat
        pos += 1
    else:
        FIFO[0] = dat
        pos = 1

    FWMA_t['len'] = x
    FWMA_t['pos'] = pos


def FWMA_avg(FWMA_t: dict, af: float, pres: float) -> float:
    '''
        求当前滤波之后的值
    '''

    FIFO: list = FWMA_t['fifo']
    '''队列'''

    x: list = FWMA_t['len']
    '''当前队列已经有多少个数据'''

    pos: int = FWMA_t['pos']
    '''当前元素的位置'''

    avg_pre: float = FWMA_t['avg_pre']
    '''上一次转换结果'''

    sum_l = 0
    sum_w = 0
    # 指数加权
    for i in range(0, x):
        w = af / (af + i)
        sum_l += w * FIFO[(i + pos - 1) % x]
        sum_w += w

    AVG = (sum_l + avg_pre * pres) / (sum_w + pres)

    # 不加权
    # AVG = sum(FIFO) / x

    FWMA_t['avg_pre'] = AVG

    return AVG


def FWMA_filter(FWMA_t: dict, af: float, pres: float, dat: float) -> float:
    '''
    FWMA滤波函数，作为整个算法的入口

    返回滤波后的结果
    '''

    count = FWMA_t['count']
    '''连续读三次才会重新计算一次'''

    buf = FWMA_t['buf']
    '''数据输入缓冲区'''

    avg_pre: float = FWMA_t['avg_pre']
    '''上一次转换结果'''

    buf[count] = dat

    if (count < 2):
        count += 1
        FWMA_t['count'] = count
        return avg_pre

    FWMA_t['count'] = 0

    # 调用FWMA_push将新数据添加到FWMA中
    FWMA_push(FWMA_t, np.median(buf))

    # 调用FWMA_avg计算滤波后的值
    avg = FWMA_avg(FWMA_t, af, pres)

    return avg


def FWMA_init(FWMA_t: dict):
    FWMA_t.clear()
    FWMA_t['fifo'] = [0] * FWMA_LEN
    FWMA_t['len'] = 0
    FWMA_t['avg_pre'] = 0
    FWMA_t['count'] = 0
    FWMA_t['pos'] = 0
    FWMA_t['buf'] = [0] * 3


def main():
    '''启动函数'''

    # 这是几个需要调节的参数

    pres = 1
    '''
        上次结果权，控制前一次计算结果对当前计算结果的影响程度，
        值越大，前一次计算结果对当前结果的影响越大，反应越慢，数据越平滑
    '''

    af = 2
    '''
        衰减系数，数值越小，输出结果更敏感于新数据的变化，
        但对噪声的去除效果可能会降低。
        较小的衰减系数适用于需要更快响应的实时应用    
    '''

    # data_min = 0
    # '''数据范围最小值'''

    # data_max = 2.5
    # '''数据范围最大值'''

    # 在 x 轴上创建一个时间序列
    tdat = np.arange(0.7, 0.8, 0.0002)

    # 生成随机噪声数据(通过修改注释，切换线性和方波数据)
    noise = [tdat[i]  # 线性
             # 1.0 if (i % 200 > 100 ) else 2.0 # 方波
             + (np.random.randn() % 0.001)  # 小浮动
             + (np.random.randn() %
                0.05 if (np.random.randn() % 1) > 0.5 else 0)  # 中浮动
             + (0.5 if (np.random.randn() % 1) > 0.99 else 0)  # 正尖刺
             + (-0.5 if (np.random.randn() % 1) > 0.99 else 0)  # 负尖刺
             for i in range(len(tdat))]

    # 绘制原始数据和滤波器输出数据
    fig, ax = plt.subplots()

    ax.plot(tdat, noise, label='input data')

    # 用 FWMA 滤波器过滤噪声数据
    FWMA_data = dict()

    FWMA_init(FWMA_data)
    filtered_data = [FWMA_filter(FWMA_data, af, pres, noise[i])
                     for i in range(len(tdat))]
    ax.plot(tdat, filtered_data, label='filter')

    ax.legend()

    plt.show()


if __name__ == '__main__':
    main()
