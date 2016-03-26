/*
* NXP tfa9887 (I2C bus)
*
*
* creat by lenovo jixu
*/

#include <linux/input.h>	/* BUS_I2C */
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/types.h>
//for platform device
#include <linux/platform_device.h>
//for misc device
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/dma-mapping.h>

//#include <linux/pm.h>
#include "nxp_tfa9887.h"
#include "../sound/AudDrv_Clk.h"

#include <mach/mt_typedefs.h>
#include <mach/mt_gpio.h>
#include <mach/mt_pm_ldo.h>

/*******************/
/*pre-def*/
#define TFA9887_CONFIG_PROC_FILE "tfa9887"
#define I2C_DEVICE_ADDRESS_LEN 2
#define I2C_MASTER_CLOCK       400
#define MAX_TRANSACTION_LENGTH 8
#define MAX_I2C_TRANSFER_SIZE (MAX_TRANSACTION_LENGTH - I2C_DEVICE_ADDRESS_LEN)
#define NXP_TFA9887_ADDR 0x68 //0x6E
#define I2C_SLAVE _IO('N',1)


/*******************/
static int tfa9887_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int tfa9887_i2c_remove(struct i2c_client *client);
static int tfa9887_platform_probe(struct platform_device *pdev);
static int tfa9887_platform_remove(struct platform_device *pdev);
static int tfa9887_misc_open(struct inode *inode, struct file *file);
static int tfa9887_misc_release(struct inode *inode, struct file *file);
static long tfa9887_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t tfa9887_write(struct file *fp, const char __user *data, size_t count, loff_t *offset);
static ssize_t tfa9887_read(struct file *fp,  char __user *data, size_t count,loff_t *offset);


static struct i2c_client *i2c_client_point = NULL;
static struct proc_dir_entry *tfa9887_config_proc = NULL;


static const struct i2c_device_id tfa9887_i2c_id[] = {{"tfa9887", 0}, {}};
static unsigned short force[] = {0, NXP_TFA9887_ADDR, I2C_CLIENT_END, I2C_CLIENT_END};//or 0x68 0x69
static const unsigned short *const forces[] = { force, NULL };	 
static struct i2c_board_info __initdata tfa9887_i2c_board_info = { I2C_BOARD_INFO("tfa9887", (NXP_TFA9887_ADDR >> 1))};

static u8 *Tfa9887I2CDMABuf_va = NULL;
static u32 Tfa9887I2CDMABuf_pa = NULL;


static struct i2c_driver tfa9887_i2c_driver =
{
    .probe = tfa9887_i2c_probe,
    .remove = tfa9887_i2c_remove,
    .driver.name = "tfa9887",
    .id_table = tfa9887_i2c_id,
    //.address_list = (const unsigned short *) forces,
};

static struct platform_driver tfa9887_platform_driver = {
	.probe      = tfa9887_platform_probe,
	.remove     = tfa9887_platform_remove,    
	.driver     = {
		.name  = "tfa9887",
		//.owner = THIS_MODULE,
	}
};

static struct file_operations tfa9887_fops = {
//	.owner = THIS_MODULE,
	.open = tfa9887_misc_open,
	.release = tfa9887_misc_release,
	.unlocked_ioctl = tfa9887_unlocked_ioctl,
	.write	= tfa9887_write,
	.read	= tfa9887_read,
};
/*----------------------------------------------------------------------------*/
static struct miscdevice tfa9887_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "tfa9887",
	.fops = &tfa9887_fops,
};
static char cmd_buf[256];

static int nxp_i2c_master_send(const struct i2c_client *client, const char *buf, int count)
{
	int ret;
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	
	msg.timing = I2C_MASTER_CLOCK;

	if(count <= 8)
	{	
		msg.addr = client->addr & I2C_MASK_FLAG;
	}
	else
	{
		msg.addr = client->addr & I2C_MASK_FLAG | I2C_DMA_FLAG;
	}	
		
	msg.flags = client->flags & I2C_M_TEN;
//	msg.timing = client->timing;

	msg.len = count;
	msg.buf = (char *)buf;
	msg.ext_flag = client->ext_flag;
	ret = i2c_transfer(adap, &msg, 1);

	/*
	 * If everything went ok (i.e. 1 msg transmitted), return #bytes
	 * transmitted, else error code.
	 */
	return (ret == 1) ? count : ret;
}

static int nxp_i2c_master_recv(const struct i2c_client *client, char *buf, int count)
{
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	int ret;

	msg.timing = I2C_MASTER_CLOCK;
	msg.flags = client->flags & I2C_M_TEN;
	msg.flags |= I2C_M_RD;
	msg.len = count;
	msg.ext_flag = client->ext_flag;
	msg.buf = (char *)buf;

	if(count <= 8)
	{
		msg.addr = client->addr & I2C_MASK_FLAG;
	}
	else
	{
		msg.addr = client->addr & I2C_MASK_FLAG | I2C_DMA_FLAG;
	}

	ret = i2c_transfer(adap, &msg, 1);

	/*
	 * If everything went ok (i.e. 1 msg received), return #bytes received,
	 * else error code.
	 */
	return (ret == 1) ? count : ret;
}

static int i2c_read_byte(struct i2c_client *client, u8 addr, u8 *data)
{
    u8 buf;
    int ret = 0;
    
    buf = addr;
    ret = nxp_i2c_master_send(client, (const char*)&buf, 1);
    if (ret < 0) {
        NXP_INFO("send command error!!\n");
        return -EFAULT;
    }
    ret = nxp_i2c_master_recv(client, (char*)&buf, 1);
    if (ret < 0) {
        NXP_INFO("reads data error!!\n");
        return -EFAULT;
    } else {
        //NXP_INFO("%s(0x%02X) = %02X\n", __func__, addr, buf);    
    }
    *data = buf;
    return 0;
}
/*----------------------------------------------------------------------------*/
static int i2c_write_byte(struct i2c_client *client, u8 addr, u8 data)
{
    u8 buf[] = {addr, data};
    int ret = 0;

    ret = nxp_i2c_master_send(client, (const char*)buf, sizeof(buf));
    if (ret < 0) {
        NXP_INFO("send command error!!\n");
        return -EFAULT;
    } else {
        //NXP_INFO("%s(0x%02X)= %02X\n", __func__, addr, data);
    }
    return 0;
}

static int i2c_read_block(struct i2c_client *client, u8 addr, u8 *data, u8 len)
{
    if (len == 1) {
        return i2c_read_byte(client, addr, data);
    } else {
        u8 beg = addr; 
        struct i2c_msg msgs[2] = {
            {
                .addr = client->addr,    .flags = 0,
                .len = 1,                .buf= &beg
            },
            {
                .addr = client->addr,    .flags = I2C_M_RD,
                .len = len,             .buf = data,
            }
        };
        int err;

        if (!client)
            return -EINVAL;
        else if (len > MAX_TRANSACTION_LENGTH) {        
            NXP_INFO(" length %d exceeds %d\n", len, MAX_TRANSACTION_LENGTH);
            return -EINVAL;
        }

        err = i2c_transfer(client->adapter, msgs, sizeof(msgs)/sizeof(msgs[0]));
        if (err != 2) {
            NXP_INFO("i2c_transfer error: (%d %p %d) %d\n", addr, data, len, err);
            err = -EIO;
        } else {
            static char buf[128];
            int idx, buflen = 0;
            for (idx = 0; idx < len; idx++)
                buflen += snprintf(buf+buflen, sizeof(buf)-buflen, "%02X ", data[idx]);
            //NXP_INFO("%s(0x%02X,%2d) = %s\n", __func__, addr, len, buf);
            err = 0;    /*no error*/
        }
        return err;
    }

}

static int i2c_write_block(struct i2c_client *client, u8 addr, u8 *data, u8 len)
{   /*because address also occupies one byte, the maximum length for write is 7 bytes*/
    int err, idx, num;
    char buf[MAX_TRANSACTION_LENGTH];

    if (!client)
        return -EINVAL;
    else if (len >= MAX_TRANSACTION_LENGTH) {        
        NXP_INFO(" length %d exceeds %d\n", len, MAX_TRANSACTION_LENGTH);
        return -EINVAL;
    }    

    num = 0;
    buf[num++] = addr;
    for (idx = 0; idx < len; idx++)
        buf[num++] = data[idx];

    err = nxp_i2c_master_send(client, buf, num);
    if (err < 0) {
        NXP_INFO("send command error!!\n");
        return -EFAULT;
    } else {
        static char buf[128];
        int idx, buflen = 0;
        for (idx = 0; idx < len; idx++)
            buflen += snprintf(buf+buflen, sizeof(buf)-buflen, "%02X ", data[idx]);
        //NXP_INFO("%s(0x%02X,%2d)= %s\n", __func__, addr, len, buf);    
        err = 0;    /*no error*/
    }
    return err;
}
static int i2c_write_dummy( struct i2c_client *client, u16 addr )
{
	int err, idx, num;

    char buf[MAX_TRANSACTION_LENGTH];
	if (!client)
		return -EINVAL;


    //NXP_INFO("i2c_write_dummy to device %02X address %04X\n", client->addr, addr );
	num = 0;
		buf[num++] = addr;

    err = nxp_i2c_master_send(client, buf, num);
    if (err < 0) {
        NXP_INFO("send command error!!\n");
        return -EFAULT;
    } else {
        //NXP_INFO("%s(0x%02X,%2d)= %s\n", __func__, addr, buf);    
        err = 0;    /*no error*/
    }

    return 0;
}

static int tfa9887_config_read_proc(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int err =0,i=0;
	u8 raw_buffer_curr[10] = {0};

	char *ptr = page;

	NXP_INFO(" %s \n", __func__ );

	ptr += sprintf( ptr, "==== tfa9887_config_read_proc====\n" );
	err = i2c_read_block(i2c_client_point,0x00,raw_buffer_curr,2);
	if (err!=0) NXP_ERROR("read error\n");
	
	for ( i = 0 ; i < 10 ; i++ )
    {
        ptr += sprintf( ptr, "0x%02X ", raw_buffer_curr[i] );

    }    
  ptr += sprintf( ptr, "\n" );
  *eof = 1;
  return ( ptr - page );
}


static int tfa9887_config_write_proc(struct file *file, const char *buffer, unsigned long count, void *data)
{
	int ret;
	int cmd, p1, p2, p3, p4, p5, p6;   
	int clk_en;

    NXP_INFO("%s write count %ld\n", __func__,count );

	if (count == 0) return -1;
	if(count > 255) count = 255;

	ret = copy_from_user(cmd_buf, buffer, count);
	if (ret < 0) {
		NXP_ERROR("copy from user error\n");
		return -1;
	}
	
	cmd_buf[count] = '\0';
	NXP_INFO("[****Debug****]Write %s\n", cmd_buf);

	sscanf(cmd_buf, "%x %x %x %x %x %x %x", &cmd, &p1, &p2, &p3, &p4, &p5, &p6);

	
	if(cmd == 0) {
		clk_en = p1;
		if(p1)
			AudDrv_Clk_On( );
		else
			AudDrv_Clk_Off( );
	}

    return count;
}

static int tfa9887_checkID(struct i2c_client *client)
{
	u8 databuf[2];
    u8 ii = 0;
	int res = 0;
	int deviceid = 0;

	memset(databuf, 0, sizeof(u8)*2);
	databuf[0] = 0x03;    
	ii = 0;
	while(ii < 5)
	{
		res = nxp_i2c_master_send(client, databuf, 0x1);
		if(res <= 0)
		{
			ii++; 
		}
        else
		{
			break;
		}
	}
	if(ii == 5)
	{
		NXP_INFO("tfa9887_checkID error: i2c write to TFA9887!\n");
		goto exit_tfa9887_checkID;
	}
        
	databuf[0] = 0x0;
	ii = 0;
	while(ii < 5)
	{        
		res = nxp_i2c_master_recv(client, databuf, 0x02);
		if(res <= 0)
		{
			ii++;
		}
		else
		{
			break;
		}
	}
	if(ii == 5)
	{
		NXP_INFO("tfa9887_checkID error: i2c read from TFA9887!\n");
		goto exit_tfa9887_checkID;
	}

	deviceid = (databuf[0]<<8)|databuf[1];
	if(deviceid!=TFA9887_DEVICEID)
	{
		NXP_INFO("fail to read tfa9887 device id\n");
		goto exit_tfa9887_checkID;
	}
	else
	{
		NXP_INFO("check tfa9887 device id = 0x%x pass\n ", deviceid);
	}

exit_tfa9887_checkID:
	if (res <= 0)
	{
		//NXP_INFO("tfa9887 I2C error, can not communication\n ");
		return -1;
	}
	return 0;
}

static int tfa9887_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int err = 0;
	u8 raw_buffer_curr[10] = {0};

	NXP_INFO("prob");

	i2c_client_point = client;

	mt_set_gpio_mode(GPIO_PA_LDO_EN, GPIO_MODE_00);
	mt_set_gpio_dir(GPIO_PA_LDO_EN, GPIO_DIR_OUT);
	mt_set_gpio_out(GPIO_PA_LDO_EN, GPIO_OUT_ONE);
	msleep(50);

	err = tfa9887_checkID(client);
	if(err < 0)
	{
		goto exit_init_failed;
	}

	tfa9887_config_proc = create_proc_entry(TFA9887_CONFIG_PROC_FILE, 0666, NULL);

	if (tfa9887_config_proc == NULL)
	{
		NXP_ERROR("create_proc_entry %s failed\n", TFA9887_CONFIG_PROC_FILE);
		goto exit_init_failed;
	}
	else
	{
		tfa9887_config_proc->read_proc = tfa9887_config_read_proc;
		tfa9887_config_proc->write_proc = tfa9887_config_write_proc;
	}

	//creat misc device
	if((err = misc_register(&tfa9887_misc_device)))
	{
		NXP_ERROR("fail to register tfa9887_misc_device\n");
		goto exit_init_failed;
	}

	Tfa9887I2CDMABuf_va = (u8 *)dma_alloc_coherent(NULL, 4096, &Tfa9887I2CDMABuf_pa, GFP_KERNEL);
	if(!Tfa9887I2CDMABuf_va)
	{
		NXP_ERROR("tfa9887 dma_alloc_coherent error\n");
		goto exit_init_failed;
	}
	NXP_INFO("tfa9887_i2c_probe success\n");
	return 0;
	
exit_init_failed:
	NXP_INFO("tfa9887_i2c_probe failed\n");
	//mt_set_gpio_mode(GPIO_PA_LDO_EN, GPIO_MODE_00);
	//mt_set_gpio_dir(GPIO_PA_LDO_EN, GPIO_DIR_IN);
	//mt_set_gpio_out(GPIO_PA_LDO_EN, GPIO_OUT_ZERO);
	return -1;
	
}

 static int tfa9887_i2c_remove(struct i2c_client *client)
 {
	if(Tfa9887I2CDMABuf_va)
	{
		dma_free_coherent(NULL, 4096, Tfa9887I2CDMABuf_va, Tfa9887I2CDMABuf_pa);
		Tfa9887I2CDMABuf_va = NULL;
		Tfa9887I2CDMABuf_pa = 0;
	}
	return 0;
 }

static int tfa9887_platform_probe(struct platform_device *pdev)
{
	if(i2c_add_driver(&tfa9887_i2c_driver))
	{
		NXP_ERROR("add i2c driver error\n");
		return -1;
	} 
	return 0;
}
/*----------------------------------------------------------------------------*/
static int tfa9887_platform_remove(struct platform_device *pdev)
{
 
	i2c_del_driver(&tfa9887_i2c_driver);
  
	return 0;
}

static int tfa9887_misc_open(struct inode *inode, struct file *file)
{

	return nonseekable_open(inode, file);
}
/*----------------------------------------------------------------------------*/
static int tfa9887_misc_release(struct inode *inode, struct file *file)
{
	return 0;
}
/*----------------------------------------------------------------------------*/

static long tfa9887_unlocked_ioctl(struct file *file, unsigned int cmd,
       unsigned long arg)
{
	
	//char strbuf[256];
	void __user *data;
	
	long err = 0;
	
	if(_IOC_DIR(cmd) & _IOC_READ)
	{
		err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
	}
	else if(_IOC_DIR(cmd) & _IOC_WRITE)
	{
		err = !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));
	}

	if(err)
	{
		NXP_ERROR(" access error: %08X, (%2d, %2d)\n", cmd, _IOC_DIR(cmd), _IOC_SIZE(cmd));
		return -EFAULT;
	}
	//NXP_INFO("cmd is 0x%x\n",cmd);
	switch(cmd)
	{
		case I2C_SLAVE:
			if(i2c_client_point->addr != arg)
				return -EINVAL;
			i2c_client_point->addr = arg;
				return 0;
		default:
			NXP_ERROR("tpd: unknown IOCTL: 0x%08x\n", cmd);
			err = -ENOIOCTLCMD;
			break;
			
	}
	return err;
}
static ssize_t tfa9887_write(struct file *file, const char __user *data, size_t count, loff_t *offset)
{
	int i = 0;
	int ret;
	char *tmp;

	//if (count > 8192)
	//	count = 8192;

	tmp = kmalloc(count,GFP_KERNEL);
	if (tmp==NULL)
		return -ENOMEM;
	if (copy_from_user(tmp,data,count)) {
		kfree(tmp);
		return -EFAULT;
	}

	//NXP_INFO("i2c-dev: i2c-%d writing %zu bytes.\n", iminor(file->f_path.dentry->d_inode), count);

	for(i = 0;  i < count; i++)
	{
		Tfa9887I2CDMABuf_va[i] = tmp[i];
	}

	if(count <= 8)
	{
		ret = nxp_i2c_master_send(i2c_client_point,tmp,count);
	}
	else
	{
		ret = nxp_i2c_master_send(i2c_client_point, Tfa9887I2CDMABuf_pa, count);
	}
	kfree(tmp);
	return ret;
}

static ssize_t tfa9887_read(struct file *file,  char __user *data, size_t count,loff_t *offset)
{
	int i = 0;
	char *tmp;
	int ret;

	if (count > 8192)
		count = 8192;

	tmp = kmalloc(count,GFP_KERNEL);
	if (tmp==NULL)
		return -ENOMEM;

	//NXP_INFO("i2c-dev: i2c-%d reading %zu bytes.\n", iminor(file->f_path.dentry->d_inode), count);

	if(count <= 8)
	{
		ret = nxp_i2c_master_recv(i2c_client_point,tmp,count);
	}
	else
	{
		ret = nxp_i2c_master_recv(i2c_client_point,Tfa9887I2CDMABuf_pa,count);
		for(i = 0; i < count; i++)
		{
			tmp[i] = Tfa9887I2CDMABuf_va[i];
		}
	}
	
	if (ret >= 0)
		ret = copy_to_user(data,tmp,count)?-EFAULT:ret;
	kfree(tmp);
	return ret;
}

/* called when loaded into kernel */
static int __init tfa9887_driver_init(void)
{
	NXP_INFO("init");

	if (i2c_register_board_info(2, &tfa9887_i2c_board_info, 1) !=0) {
		NXP_ERROR(" cann't register i2c\n");
		return -1;
	}
	if(platform_driver_register(&tfa9887_platform_driver))
	{
		NXP_ERROR("failed to register platform driver");
		return -1;
	}

	return 0;
}

/* should never be called */
static void __exit tfa9887_driver_exit(void)
{
	NXP_INFO("exit");

	i2c_del_driver(&tfa9887_i2c_driver);
}

module_init(tfa9887_driver_init);
module_exit(tfa9887_driver_exit);

